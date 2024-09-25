#include "Quad.h"

#include "imgui.h"
#include "imgui_stdlib.h"
#include "Core/App.h"
#include "Core/Timer.h"
#include "UI/HPImGui.h"


Quad::Quad()
{

    VertexBufferLayout layout = {
        {ShaderDataType::Float3, "Position"},
        {ShaderDataType::Float3, "Color"},
        {ShaderDataType::Float2, "TexCoord"},
        {ShaderDataType::Float3, "Normal"},
    };

    
    
    
    vb.reset(new GLVertexBuffer(vertices, sizeof(vertices)));
    va.reset(new GLVertexArray());
    ib.reset(new GLIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));
    
    vb->SetLayout(layout);
    va->AddVertexBuffer(vb);
    va->SetIndexBuffer(ib);
    
    shader = Shader::Create("Resources/Shaders/VertexColor.vs", "Resources/Shaders/TexturedShader.fs");
    shader->Bind();
    

    texture = Texture::CreateTexture("Resources/Textures/beetho.png");
    texture->Bind(0);

    vb->Unbind();
    va->Unbind();
    ib->Unbind();
    shader->Unbind();

    SetRenderSettings({GL_TRIANGLES, false, true, false, true});
}

Quad::~Quad()
{
}


void Quad::Draw()
{
    texture->Bind(0);
    Renderer::Submit(shader, va, m_transform, {texture}, m_renderSettings);

}

void Quad::Update(Timestep ts)
{
    Actor::Update(ts);
}

void Quad::Begin()
{
}

void Quad::OnDestroy()
{
}

Texture* Quad::SetTexture(const std::string& path)
{
    texture = (Texture::CreateTexture(path));
    m_texturePath = path;
    return texture;
}

nlohmann::json Quad::Serialize()
{
    nlohmann::json j = Actor::Serialize();
    j["TexturePath"] = m_texturePath;
    j["TextureFiltering"] = texture->GetFilteringMode();
    return j;
}

void Quad::Deserialize(nlohmann::json& j)
{
    Actor::Deserialize(j);
    std::string path = j["TexturePath"];
    if (!path.empty())
    SetTexture(path);
    if (j.contains("TextureFiltering"))
        texture->SetFilteringMode(j["TextureFiltering"]);
}

void Quad::OnInspectorGUI()
{
    if (ImGui::OpenFilePath("Texture", m_texturePath, "Load Texture", "Image File (*.png;*.jpg;*.jpeg){.png,.jpg,.jpeg},.*", "Resources/Textures"))
    {
        SetTexture(m_texturePath);
    }

    ImGui::TextureFilteringSelector("Filtering Mode", texture);
    

    ImGui::ImageScaledH(texture, 0.5f, true);
}
