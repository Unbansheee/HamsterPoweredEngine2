#include "MeshActor.h"
#include "ResourceManagement/Texture.h"
#include <iostream>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "UI/HPImGui.h"
#include <assimp/postprocess.h>

MeshActor::MeshActor()
{
    textures.resize(32, nullptr);
    texturepaths.resize(32, "");
    
    VertexBufferLayout layout = {
        {ShaderDataType::Float3, "Position"},
        {ShaderDataType::Float3, "Color"},
        {ShaderDataType::Float2, "TexCoord"},
        {ShaderDataType::Float3, "Normal"}
    };
    
    vb.reset(new GLVertexBuffer(vertices.data(), (uint32_t)vertices.size() * sizeof(float)));
    va.reset(new GLVertexArray());
    ib.reset(new GLIndexBuffer(indices.data(), (int)indices.size()));
    
    vb->SetLayout(layout);
    va->AddVertexBuffer(vb);
    va->SetIndexBuffer(ib);
    
    shader = Shader::Create("Resources/Shaders/VertexColor.vs", "Resources/Shaders/TexturedShader.fs");
    shader->Bind();


    textures[0] = Texture::CreateTexture("Resources/Textures/beetho.png");
    textures[0]->Bind(0);
    
    vb->Unbind();
    va->Unbind();
    ib->Unbind();
    shader->Unbind();

    SetRenderSettings({GL_TRIANGLES, false, true, false, true});
    
}

MeshActor::~MeshActor()
{
    
    
}

void MeshActor::OnInspectorGUI()
{
    Actor::OnInspectorGUI();


    if (ImGui::OpenFilePath("Mesh", meshpath, "Load Mesh", "3D Mesh File (*.obj;*.fbx){.obj,.fbx},.*", "Resources/Meshes"))
    {
        LoadMesh(meshpath);
    }
    
    if (ImGui::CollapsingHeader("Textures"))
    {
        for (int i = 0; i < textures.size(); i++)
        {
            ImVec2 cursor = ImGui::GetCursorPos();
            bool textureOpen = ImGui::CollapsingHeader((std::string("Slot ") + std::to_string(i)).c_str());
            if (textures[i])
            {
                ImVec2 dim = ImGui::GetItemRectSize();
                float scalingRation =  dim.y / (float)textures[i]->GetHeight();
                ImVec2 imageSize = {(float)textures[i]->GetWidth() * scalingRation, dim.y};
                ImGui::SameLine(ImGui::GetWindowWidth() - imageSize.x*2);
                ImGui::Image(textures[i], imageSize);
            }
            if (textureOpen)
            {
                if (ImGui::OpenFilePath(("Path##" + std::to_string(i)).c_str(), texturepaths[i], "Load Texture (Slot " + std::to_string(i) + ")", "Image File (*.png;*.jpg;*.jpeg){.png,.jpg,.jpeg},.*", "Resources/Textures"))
                {
                    SetTexture(texturepaths[i], i);
                }
                
                if (textures[i])
                {
                    ImGui::TextureFilteringSelector("Filtering Mode", textures[i]);
                    ImGui::ImageScaledH(textures[i], 0.25f);
                }
            }
        }
    }
    
}

void MeshActor::Draw()
{
    
    Actor::Draw();
    Renderer::Submit(shader, va, m_transform, textures);
    
    
}

void MeshActor::Update(Timestep ts)
{
    Actor::Update(ts);
}

void MeshActor::Begin()
{
}

void MeshActor::OnDestroy()
{
}

Texture* MeshActor::SetTexture(const std::string& path, int slot)
{
    //check if slot is valid
    if (slot < 0 || slot > 31)
    {
        std::cout << "Texture slot must be between 0 and 31" << std::endl;
        return nullptr;
    }
    textures[slot]->Bind(slot);
    textures[slot] = (Texture::CreateTexture(path));
    texturepaths[slot] = path;
    textures[slot]->Unbind();
    return textures[slot];
}

void MeshActor::SetMesh(float* _vertices, uint32_t _size, uint32_t* _indices, uint32_t _isize)
{
    vertices.clear();
    indices.clear();
    for (int i = 0; i < _size / (sizeof(float) * 11); i++)
    {
        vertices.push_back(_vertices[i]);
    }
     for (int i = 0; i < _isize; i++)
    {
        indices.push_back(_indices[i]);
    }
    
    
    VertexBufferLayout layout = {
        {ShaderDataType::Float3, "Position"},
        {ShaderDataType::Float3, "Color"},
        {ShaderDataType::Float2, "TexCoord"},
        {ShaderDataType::Float3, "Normal"},
    };
    
    
    vb.reset(new GLVertexBuffer(vertices.data(), (uint32_t)vertices.size() * sizeof(float)));
    va.reset(new GLVertexArray());
    ib.reset(new GLIndexBuffer(indices.data(), (int)indices.size()));
    
    vb->SetLayout(layout);
    va->AddVertexBuffer(vb);
    va->SetIndexBuffer(ib);
    
    
    vb->Unbind();
    va->Unbind();
    ib->Unbind();
    shader->Unbind();

    UpdateTransform();
}

void MeshActor::CopyNodesWithMeshes(aiNode* node, aiMatrix4x4 accTransform)
{
    aiMatrix4x4 transform;
    if (node->mNumMeshes > 0)
    {
        for (unsigned index = 0; index < node->mNumMeshes; index++)
        {
            //get mesh from node
            aiMesh* mesh = scene->mMeshes[node->mMeshes[index]];
            //get transform from node
            //transform = node->mTransformation;
            //multiply transform by accumulated transform
            //transform = accTransform * transform;
            
            for (unsigned i = 0; i < mesh->mNumVertices; i++)
            {
                aiVector3D pos = mesh->mVertices[i];
                aiVector3D transformed = transform * pos;
                
                vertices.push_back(transformed.x);
                vertices.push_back(transformed.y);
                vertices.push_back(transformed.z);
                vertices.push_back(mesh->HasVertexColors(0) ? mesh->mColors[0][i].r : 1.0f);
                vertices.push_back(mesh->HasVertexColors(0) ? mesh->mColors[0][i].g : 1.0f);
                vertices.push_back(mesh->HasVertexColors(0) ? mesh->mColors[0][i].b : 1.0f);
                aiVector3D textCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f);
                vertices.push_back(textCoord.x);
                vertices.push_back(textCoord.y);
                vertices.push_back(mesh->HasNormals() ? mesh->mNormals[i].x : 0.0f);
                vertices.push_back(mesh->HasNormals() ? mesh->mNormals[i].y : 0.0f);
                vertices.push_back(mesh->HasNormals() ? mesh->mNormals[i].z : 0.0f);
            }

            for (unsigned i = 0; i < mesh->mNumFaces; i++)
            {
                indices.push_back(mesh->mFaces[i].mIndices[0]);
                indices.push_back(mesh->mFaces[i].mIndices[1]);
                indices.push_back(mesh->mFaces[i].mIndices[2]);
            }
        }

        
    }
    else
    {
        //transform = node->mTransformation * accTransform;
    }

    for (int i = 0; i < node->mNumChildren; i++)
    {
        CopyNodesWithMeshes(node->mChildren[i], transform);
    }
}

void MeshActor::LoadMesh(const std::string& path)
{
    
    Assimp::Importer importer;
    scene = importer.ReadFile(path, aiProcess_Triangulate);
    if (!scene)
    {
        std::cout << "Error loading mesh. File not found: " << path << std::endl;
        return;
    }

    if (!scene->mMeshes[0])
    {
        std::cout << "Error loading mesh. No meshes found: " << path << std::endl;
        return;
    }

    meshpath = path;
    
    vertices.clear();
    indices.clear();

    CopyNodesWithMeshes(scene->mRootNode, aiMatrix4x4());
    
    VertexBufferLayout layout = {
        {ShaderDataType::Float3, "Position"},
        {ShaderDataType::Float3, "Color"},
        {ShaderDataType::Float2, "TexCoord"},
        {ShaderDataType::Float3, "Normal"},
    };
    
    
    vb.reset(new GLVertexBuffer(vertices.data(), (uint32_t)vertices.size() * sizeof(float)));
    va.reset(new GLVertexArray());
    ib.reset(new GLIndexBuffer(indices.data(), (int)indices.size()));
    
    vb->SetLayout(layout);
    va->AddVertexBuffer(vb);
    va->SetIndexBuffer(ib);
    
    
    vb->Unbind();
    va->Unbind();
    ib->Unbind();
    shader->Unbind();
    
    
    importer.FreeScene();
    UpdateTransform();
    
}

void MeshActor::Deserialize(nlohmann::json& j)
{
    Actor::Deserialize(j);
    meshpath = j["MeshPath"];
    if (!meshpath.empty()) LoadMesh(meshpath);
    
    for (int i = 0; i < j["Textures"].size(); i++)
    {
        texturepaths[i] = j["Textures"][i];
        if (!texturepaths[i].empty()) SetTexture(texturepaths[i], i);
    }
    
    
}

nlohmann::json MeshActor::Serialize()
{
    auto j = Actor::Serialize();
    j["MeshPath"] = meshpath;
    j["Textures"] = texturepaths;
    return j;
}

