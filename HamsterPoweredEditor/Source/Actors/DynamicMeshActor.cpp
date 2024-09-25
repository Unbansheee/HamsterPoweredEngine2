#include "DynamicMeshActor.h"

#include "UI/HPImGui.h"

DynamicMeshActor::DynamicMeshActor()
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    //create a plane
    vertices.push_back(Vertex(glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0));
    vertices.push_back(Vertex(glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0));
    vertices.push_back(Vertex(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0));
    vertices.push_back(Vertex(glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    
    m_mesh.Create(vertices, indices);
}

void DynamicMeshActor::Update(Timestep ts)
{
    Actor::Update(ts);
}

void DynamicMeshActor::OnDestroy()
{
    Actor::OnDestroy();
}

nlohmann::json DynamicMeshActor::Serialize()
{
    auto j = Actor::Serialize();
    j["TexturePath"] = texturePath;
    return j;
}

void DynamicMeshActor::Deserialize(nlohmann::json& j)
{
    Actor::Deserialize(j);
    if (j.contains("TexturePath")) texturePath = j["TexturePath"];
    m_mesh.SetTexture(texturePath);
    
}

void DynamicMeshActor::OnInspectorGUI()
{
    Actor::OnInspectorGUI();

    if (ImGui::OpenFilePath("Texture", texturePath, "Open File", "Image File (*.png;*.jpg;*.jpeg){.png,.jpg,.jpeg},.*", "Resources/Textures"))
    {
        m_mesh.SetTexture(texturePath);
        
    }
}

DynamicMeshActor::~DynamicMeshActor()
{
}

void DynamicMeshActor::Draw()
{
    Actor::Draw();
    m_mesh.Draw(m_transform);
}

void DynamicMeshActor::Begin()
{
    Actor::Begin();

    

    
}
