#include "TextLabel.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "Core/App.h"
#include "Core/Input.h"
#include "Core/Window.h"
#include "UI/HPImGui.h"

unsigned int indices[3 * 2] = {
    0, 1, 2,   // first triangle
    0, 2, 3    // second triangle
};  

TextLabel::TextLabel(const std::string& text, const std::string& fontPath, int fontSize, const glm::vec3& color,
                     const glm::vec3& position)
{
    VertexBufferLayout layout = {
        {ShaderDataType::Float3, "Position"},
        {ShaderDataType::Float3, "Color"},
        {ShaderDataType::Float2, "TexCoord"},
        {ShaderDataType::Float, "TextureID"}
    };

    m_BaseColor = color;
    m_Text = text;
    m_FontSize = fontSize;
    m_FontPath = fontPath;
    m_Font = Font::LoadFont(fontPath, fontSize);


    m_Projection = glm::ortho(0.0f, (float)App::Instance().window->GetWidth(), 0.0f, (float)App::Instance().window->GetHeight());
    m_Shader = (Shader::Create("Resources/Shaders/Text.vs", "Resources/Shaders/Text.fs"));

    m_VBO.reset(new GLVertexBuffer(nullptr, sizeof(float) * 9 * 4 * 1000, GL_DYNAMIC_DRAW));
    m_VAO.reset(new GLVertexArray());
    m_IBO.reset(new GLIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t) * 256, GL_DYNAMIC_DRAW));
    
    m_VBO->SetLayout(layout);
    m_VAO->AddVertexBuffer(m_VBO);
    m_VAO->SetIndexBuffer(m_IBO);
    
    
    SetText(text);
    SetCurrentColor(color);
    SetPosition(position);
    
    m_Initialized = true;

    SetRenderSettings({GL_TRIANGLES, true, true, false, true});

    m_VAO->Unbind();
    m_VBO->Unbind();
    m_IBO->Unbind();
}

void TextLabel::SetText(const std::string& text)
{
    m_Text = text;
    UpdateBuffers();
}

void TextLabel::SetCurrentColor(const glm::vec3& color)
{
    m_Color = color;
    UpdateBuffers();
}

void TextLabel::SetBaseColor(const glm::vec3& color)
{
    m_BaseColor = color;
}

void TextLabel::SetHoverColor(const glm::vec3& color)
{
    m_HoverColor = color;
}

void TextLabel::SetClickColor(const glm::vec3& color)
{
    m_PressedColor = color;
}

void TextLabel::SetFont(Font* font)
{
    m_Font = font;
    UpdateBuffers();

}

Font* TextLabel::GetFont() const
{
    return m_Font;
}

void TextLabel::Begin()
{
}

void TextLabel::OnDestroy()
{
}

void TextLabel::Update(Timestep ts)
{
    Actor::Update(ts);

    
    glm::vec2 MousePosAbsolute = Input::GetMousePositionAbsolute();
    glm::vec2 viewportSize = Renderer::GetViewportSize();
    glm::vec2 viewportLocationAbsolute = Renderer::GetViewportPosition();
    
    glm::vec2 mousePosInViewport = MousePosAbsolute - viewportLocationAbsolute;
    mousePosInViewport.y = viewportSize.y - mousePosInViewport.y;
    
    
    
    
    
    //Check if mouse intersects bounds
    if (mousePosInViewport.x > m_bounds.x && mousePosInViewport.x < m_bounds.x + m_bounds.width &&
        mousePosInViewport.y > m_bounds.y && mousePosInViewport.y < m_bounds.y + m_bounds.height)
    {
        mouseOverlapping = true;
        SetCurrentColor(m_HoverColor);
        
        if (Input::WasMouseButtonPressed(Mouse::Left))
        {
            mouseoffset = mousePosInViewport - glm::vec2(m_bounds.x, m_bounds.y);
            m_Dragging = true;
        }

        
    }
    else if (mouseOverlapping)
    {
        SetCurrentColor(m_BaseColor);
        mouseOverlapping = false;
    }
    else SetCurrentColor(m_BaseColor);
    
    
    
    if (m_Dragging)
    {
        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {SetScale(GetScale() * 1.1f); mouseoffset *= 1.1f; }
        if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {SetScale(GetScale() * 0.9f); mouseoffset *= 0.9f; }
        SetPosition({ mousePosInViewport.x - mouseoffset.x, mousePosInViewport.y - mouseoffset.y, 0.0f });
        SetCurrentColor(m_PressedColor);
    }
    if (!Input::IsMouseButtonDown(Mouse::Left) && m_Dragging)
    {
        m_Dragging = false;
        SetCurrentColor(m_BaseColor);
    }
    
    m_scaleBounceDelta = (sin(App::Instance().GetTime() * 3.f) + 2.0f) * 0.5f;

    if (m_sideScroll)
    {
        // Scrolling text
        SetPosition({ GetPosition().x + m_scrollSpeed.x * ts.GetSeconds(), GetPosition().y + m_scrollSpeed.y * ts.GetSeconds(), GetPosition().z });
        if (GetPosition().y > Renderer::GetViewportSize().y)
            SetPosition({ GetPosition().x, -m_bounds.height, GetPosition().z });
        if (GetPosition().x > Renderer::GetViewportSize().x)
            SetPosition({ -m_bounds.width, GetPosition().y, GetPosition().z });
    }
}

void TextLabel::Draw()
{
    Actor::Draw();

    m_Projection = glm::ortho(0.0f, Renderer::GetViewportSize().x, 0.0f, Renderer::GetViewportSize().y);
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("ProjectionMat", m_Projection);

    m_bounds.x = GetPosition().x;
    m_bounds.y = GetPosition().y;
    m_bounds.height = m_unscaledBounds.height * GetScale().y;
    m_bounds.width = m_unscaledBounds.width * GetScale().x;

    glm::mat4 scaleTransform = m_transform;
    if (!m_Dragging && mouseOverlapping && m_scaleBounce)
    {

        scaleTransform = glm::translate(scaleTransform, glm::vec3(m_unscaledBounds.width / 2.f, m_unscaledBounds.height / 2.f, 0.0f));     // Translate to center
        scaleTransform = glm::scale(scaleTransform, m_scaleBounceDelta * glm::vec3(1, 1, 0));                                              // Scale 
        scaleTransform = glm::translate(scaleTransform, glm::vec3(-m_unscaledBounds.width / 2.f, -m_unscaledBounds.height / 2.f, 0.0f));   // Translate back to original position
        
    }
    m_Shader->Unbind();
    
    Renderer::Submit(m_Shader, m_VAO, scaleTransform, textures, m_renderSettings);
    
}

void TextLabel::OnInspectorGUI()
{
    Actor::OnInspectorGUI();
    if (ImGui::InputText("Text", &m_Text))
    {
        SetText(m_Text);
    }

    ImGui::Checkbox("Bounce", &m_scaleBounce);
    ImGui::Checkbox("Scroll", &m_sideScroll);
    ImGui::DragFloat2("Scroll Speed", &m_scrollSpeed.x, 1.f, 0.0f, 10000.0f);
    
    ImGui::Checkbox("Screen Space", &m_ScreenSpace);
    
    glm::vec3 color = m_BaseColor;
    if (ImGui::ColorEdit3("Base Color", &color[0]))
    {
        SetBaseColor(color);
    }

    glm::vec3 hovercolor = m_HoverColor;
    if (ImGui::ColorEdit3("Hover Color", &hovercolor[0]))
    {
        SetHoverColor(hovercolor);
    }

    glm::vec3 pressedcolor = m_PressedColor;
    if (ImGui::ColorEdit3("Pressed Color", &pressedcolor[0]))
    {
        SetClickColor(pressedcolor);
    }

    Font* font = m_Font;
    if (ImGui::FontSelector("Font", &font))
    {
        SetFont(font);
    }
    
}

void TextLabel::UpdateBuffers()
{
    
    float maxwidth = 0.f, maxheight = 0.f;
    glm::vec3 CharacterOrigin = {0, 0, 0};
    textures.clear();

    m_IBO->Bind();
    m_VBO->Bind();
    
    int index = 0;
    for (std::string::const_iterator TextCharacter = m_Text.begin(); TextCharacter != m_Text.end(); TextCharacter++)
    {
        Font::FontChar FontCharacter = m_Font->GetChar(*TextCharacter);
        float PosX = CharacterOrigin.x + FontCharacter.Bearing.x;
        float PosY = CharacterOrigin.y - (FontCharacter.texture->GetHeight() - FontCharacter.Bearing.y);
        float Width = FontCharacter.texture->GetWidth();
        float Height = FontCharacter.texture->GetHeight();

        int texindex = 0;
        for (int i = 0; i < textures.size(); i++)
        {
            if (textures[i] == FontCharacter.texture)
            {
                texindex = i;
                break;
            }
        }
        if (texindex == 0)
        {
            textures.push_back(FontCharacter.texture);
            texindex = textures.size() - 1;
        }
        
        float vertices[4][9] = {
            {PosX, PosY + Height, 0.0f, m_Color.x, m_Color.y, m_Color.z, 0.0f, 0.0f, (float)texindex},
            {PosX, PosY, 0.0f, m_Color.x, m_Color.y, m_Color.z,  0.0f, 1.0f, (float)texindex},
            {PosX + Width, PosY, 0.0f, m_Color.x, m_Color.y, m_Color.z, 1.0f, 1.0f, (float)texindex},
            {PosX + Width, PosY + Height, 0.0f, m_Color.x, m_Color.y, m_Color.z, 1.0f, 0.0f, (float)texindex}
        };

        // offset indices
        uint32_t _indices[6];
        for (int i = 0; i < 6; i++)
        {
            _indices[i] = indices[i] + index * 4;
        }
        
        m_VBO->UpdateData(*vertices, sizeof(vertices), index * 4 * sizeof(float) * 9);
        m_IBO->UpdateData(_indices, sizeof(_indices) / sizeof(uint32_t), index * 6 * sizeof(uint32_t));


        CharacterOrigin.x += (FontCharacter.Advance >> 6);

        maxheight = std::max(m_bounds.height, Height);
        maxwidth += (FontCharacter.Advance >> 6);
        index++;
    }
    //m_VBO->Unbind();
   // m_IBO->Unbind();
    
    
    m_unscaledBounds.height = maxheight;
    m_unscaledBounds.width = maxwidth;
}

TextLabel::~TextLabel()
{
}

nlohmann::json TextLabel::Serialize()
{
    auto json = Actor::Serialize();
    json["Text"] = m_Text;
    json["FontPath"] = m_FontPath;
    json["ScreenSpace"] = m_ScreenSpace;
    json["BaseColor"] = m_BaseColor;
    json["HoverColor"] = m_HoverColor;
    json["PressedColor"] = m_PressedColor;
    json["ScaleBounce"] = m_scaleBounce;
    json["ScrollText"] = m_sideScroll;
    json["ScrollSpeed"] = m_scrollSpeed;
    return json;
}

void TextLabel::Deserialize(nlohmann::json& j)
{
    Actor::Deserialize(j);
    m_Text = j["Text"];
    SetText(m_Text);
    m_FontPath = j["FontPath"];
    SetFont(Font::LoadFont(m_FontPath, 100));
    m_ScreenSpace = j["ScreenSpace"];
    m_BaseColor = j["BaseColor"];
    m_HoverColor = j["HoverColor"];
    m_PressedColor = j["PressedColor"];
    if (j.contains("ScaleBounce"))
    {
        m_scaleBounce = j["ScaleBounce"];
    }
    if (j.contains("ScrollText"))
    {
        m_sideScroll = j["ScrollText"];
    }
    if (j.contains("ScrollSpeed"))
    {
        m_scrollSpeed = j["ScrollSpeed"];
    }

    
}

void TextLabel::FixedUpdate(double ts)
{
}
