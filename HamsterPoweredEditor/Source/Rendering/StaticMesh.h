#pragma once
#include <vector>
#include <assimp/scene.h>
#include <glm/fwd.hpp>

#include "GLVertexArray.h"
#include "Renderer.h"
#include "Vertex.h"
#include "ResourceManagement/Shader.h"

struct aiScene;

class StaticMesh
{
public:
    StaticMesh();
    StaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    void Draw(const glm::mat4& transform);
    void Create(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    void Load(const std::string& path);
    void Clear();
    void SetShader(std::shared_ptr<Shader> shader);
    const std::shared_ptr<Shader>& GetShader() const {return m_shader;}
    const Vertex& GetClosestPoint(const glm::vec3& point);
    Texture* SetTexture(const std::string& path, int slot = 0);
    void SetTexture(Texture* texture, int slot = 0);
    std::shared_ptr<GLVertexArray> GetVAO() const {return m_vertexArray;}
    std::shared_ptr<GLVertexBuffer> GetVBO() const {return m_vertexBuffer;}
    std::shared_ptr<GLIndexBuffer> GetIBO() const {return m_indexBuffer;}
    
    const std::vector<Vertex>& GetVertices() const { return m_vertices; }
    const std::vector<uint32_t>& GetIndices() const { return m_indices; }

    void Cube();
    
protected:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;

    std::shared_ptr<GLVertexArray> m_vertexArray;
    std::shared_ptr<GLVertexBuffer> m_vertexBuffer;
    std::shared_ptr<GLIndexBuffer> m_indexBuffer;
    std::shared_ptr<Shader> m_shader;
    std::vector<Texture*> m_textures;
    std::vector<std::string> m_texturePaths;

    std::string m_path;
    const aiScene* scene;
    uint32_t m_DrawMode = GL_TRIANGLES;
    RenderSettings m_renderSettings;
    
    void CopyNodesWithMeshes(aiNode* node, aiMatrix4x4 accumulatedTransform);

};
