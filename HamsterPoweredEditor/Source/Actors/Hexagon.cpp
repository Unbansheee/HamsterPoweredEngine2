#include "Hexagon.h"

#include <glm/ext/matrix_transform.hpp>

#include "imgui.h"
#include "UI/HPImGui.h"

Hexagon::Hexagon()
{
    VertexBufferLayout layout = {
        {ShaderDataType::Float3, "Position"},
        {ShaderDataType::Float3, "Color"},
        {ShaderDataType::Float2, "TexCoord"},
        {ShaderDataType::Float3, "Normal"}
    };


    
    
    vb.reset(new GLVertexBuffer(vertices, sizeof(vertices)));
    va.reset(new GLVertexArray());
    ib.reset(new GLIndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));
    
    vb->SetLayout(layout);
    va->AddVertexBuffer(vb);
    va->SetIndexBuffer(ib);
    
    shader = Shader::Create("Resources/Shaders/VertexColor.vs", "Resources/Shaders/TexturedShader.fs");
    shader->Bind();

    texture = (Texture::CreateTexture("Resources/Textures/beetho.png"));
    texture->Bind();

    texture2 = (Texture::CreateTexture("Resources/Textures/Wood.jpg"));
    texture2->Bind(1);

    vb->Unbind();
    va->Unbind();
    ib->Unbind();
    shader->Unbind();

    SetRenderSettings({GL_TRIANGLES, false, true, false, true});
}

Hexagon::~Hexagon()
{
}

void Hexagon::Draw()
{
    Actor::Draw();
    shader->Bind();
    Renderer::Submit(shader, va, m_transform, {texture, texture2}, m_renderSettings);
    for (const auto& vec : m_InstancePositions)
    {
        glm::mat4 duplicate = glm::translate(m_transform, vec);
        Renderer::Submit(shader, va, duplicate, {texture, texture2}, m_renderSettings);
    }

}

void Hexagon::Update(Timestep ts)
{
    Actor::Update(ts);
}

void Hexagon::Begin()
{
}

void Hexagon::OnDestroy()
{
}

Texture* Hexagon::SetTexture(const std::string& path)
{
    texture = (Texture::CreateTexture(path));
    return texture;
}

void Hexagon::SetTexture(const std::string& path, const std::string& path2)
{
    texture = (Texture::CreateTexture(path));
    texture2 = (Texture::CreateTexture(path2));
}

void Hexagon::OnInspectorGUI()
{
    Actor::OnInspectorGUI();
    
    if (ImGui::CollapsingHeader("Hexagon Instances"))
    {
        int index = 0;
        for (auto& i : m_InstancePositions)
        {
            if (ImGui::Button(("x##" + std::to_string(index)).c_str()))
            {
                m_InstancePositions.erase(m_InstancePositions.begin() + index);
            }
            ImGui::SameLine();
            ImGui::DragFloat3(std::to_string(index).c_str(), &i.x, 0.1f);
            index++;
        }
        if(ImGui::Button("+"))
        {
            m_InstancePositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        }
        ImGui::Separator();
    }
}

nlohmann::json Hexagon::Serialize()
{
    auto j = Actor::Serialize();
    j["Texture"] = texture->GetPath();
    j["Texture2"] = texture2->GetPath();
    j["InstancePositions"] = m_InstancePositions;
    return j;
}

void Hexagon::Deserialize(nlohmann::json& json)
{
    Actor::Deserialize(json);
    texture = (Texture::CreateTexture(json["Texture"]));
    texture2 = (Texture::CreateTexture(json["Texture2"]));
    m_InstancePositions = json["InstancePositions"];

}
