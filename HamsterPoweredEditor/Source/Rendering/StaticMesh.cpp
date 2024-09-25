#include "StaticMesh.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Renderer.h"
#include "ResourceManagement/Texture.h"

StaticMesh::StaticMesh()
{
    m_textures.resize(32, nullptr);
    m_texturePaths.resize(32, "");
    Create({}, {});
    m_renderSettings.Culling = true;
}

StaticMesh::StaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    m_textures.resize(32, nullptr);
    m_texturePaths.resize(32, "");
    Create(vertices, indices);
    m_renderSettings.Culling = true;

}

void StaticMesh::Draw(const glm::mat4& transform)
{
    Renderer::Submit(m_shader, m_vertexArray, transform, m_textures, m_renderSettings);
}

void StaticMesh::Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    m_vertices = vertices;
    m_indices = indices;

    m_shader = Shader::Create("Resources/Shaders/VertexColor.vs", "Resources/Shaders/TexturedShader.fs");
    m_shader->Bind();
    
    m_textures[0] = Texture::CreateTexture("Resources/Textures/beetho.png");
    m_textures[0]->Bind(0);
    
    m_vertexArray.reset(new GLVertexArray());
    m_vertexBuffer.reset(new GLVertexBuffer(m_vertices, GL_DYNAMIC_DRAW));
    m_indexBuffer.reset(new GLIndexBuffer(m_indices, GL_DYNAMIC_DRAW));

    VertexBufferLayout layout = {
        {ShaderDataType::Float3, "Position"},
        {ShaderDataType::Float3, "Color"},
        {ShaderDataType::Float2, "TexCoord"},
        {ShaderDataType::Float3, "Normal"},
        {ShaderDataType::Int, "TexIndex"}
    };

    m_vertexBuffer->SetLayout(layout);
    m_vertexArray->AddVertexBuffer(m_vertexBuffer);
    m_vertexArray->SetIndexBuffer(m_indexBuffer);

    m_vertexBuffer->Unbind();
    m_vertexArray->Unbind();
    m_indexBuffer->Unbind();
    m_shader->Unbind();
    
}

void StaticMesh::Load(const std::string& path)
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

    m_path = path;
    
    m_vertices.clear();
    m_indices.clear();

    CopyNodesWithMeshes(scene->mRootNode, aiMatrix4x4());
    
    Create(m_vertices, m_indices);
    
    importer.FreeScene();
}

void StaticMesh::Clear()
{
    m_vertices.clear();
    m_indices.clear();

    Create(m_vertices, m_indices);
}

void StaticMesh::SetShader(std::shared_ptr<Shader> shader)
{
    m_shader = shader;
}

const Vertex& StaticMesh::GetClosestPoint(const glm::vec3& point)
{
    float minDistance = std::numeric_limits<float>::max();
    int minIndex = 0;

    for (int i = 0; i < m_vertices.size(); i++)
    {
        float distance = glm::distance(point, m_vertices[i].Position);
        if (distance < minDistance)
        {
            minDistance = distance;
            minIndex = i;
        }
    }

    return m_vertices[minIndex];
}


Texture* StaticMesh::SetTexture(const std::string& path, int slot)
{
    //check if slot is valid
    if (slot < 0 || slot > 31)
    {
        std::cout << "Texture slot must be between 0 and 31" << std::endl;
        return nullptr;
    }
    //m_textures[slot]->Bind(slot);
    m_textures[slot] = (Texture::CreateTexture(path));
    m_texturePaths[slot] = path;
    //m_textures[slot]->Unbind();
    return m_textures[slot];
}

void StaticMesh::SetTexture(Texture* texture, int slot)
{
    m_textures[slot] = texture;
    m_texturePaths[slot] = texture->GetPath();
}

void StaticMesh::Cube()
{
    m_vertices.clear();
    m_indices.clear();

    //Front
    m_vertices.push_back({ {-0.5f,  0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, 0 });
    m_vertices.push_back({ {-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, 0 });
    m_vertices.push_back({ {0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, 0 });
    m_vertices.push_back({ {0.5f,  0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, 0 });

    //Back
    m_vertices.push_back({ {0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, 0 });
    m_vertices.push_back({ {0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, 0 });
    m_vertices.push_back({ {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, 0 });
    m_vertices.push_back({ {-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, 0 });

    //Right
    m_vertices.push_back({ {0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, 0 });
    m_vertices.push_back({ {0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, 0 });
    m_vertices.push_back({ {0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, 0 });
    m_vertices.push_back({ {0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, 0 });

    //Left
    m_vertices.push_back({ {-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, 0 });
    m_vertices.push_back({ {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, 0 });
    m_vertices.push_back({ {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, 0 });
    m_vertices.push_back({ {-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, 0 });

    //Top
    m_vertices.push_back({ {-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, 0 });
    m_vertices.push_back({ {-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 0 });
    m_vertices.push_back({ {0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 0 });
    m_vertices.push_back({ {0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, 0 });

    //bottom
    m_vertices.push_back({ {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, 0 });
    m_vertices.push_back({ {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, 0 });
    m_vertices.push_back({ {0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, 0 });
    m_vertices.push_back({ {0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, 0 });
    

    m_indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    Create(m_vertices, m_indices);
    
    
}


void StaticMesh::CopyNodesWithMeshes(aiNode* node, aiMatrix4x4 accumulatedTransform)
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

                Vertex vertex;
                vertex.Position = { transformed.x, transformed.y, transformed.z };
                if (mesh->HasVertexColors(0)) vertex.Colour = { mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b };
                vertex.TexCoord = mesh->HasTextureCoords(0) ? glm::vec2{mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y}  : glm::vec2(0.0f, 0.0f);
                vertex.Normal = mesh->HasNormals() ? glm::vec3{mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z} : glm::vec3(0.0f, 0.0f, 0.0f);

                m_vertices.emplace_back(vertex);
            }

            for (unsigned i = 0; i < mesh->mNumFaces; i++)
            {
                m_indices.push_back(mesh->mFaces[i].mIndices[0]);
                m_indices.push_back(mesh->mFaces[i].mIndices[1]);
                m_indices.push_back(mesh->mFaces[i].mIndices[2]);
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
