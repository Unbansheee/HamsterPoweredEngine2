#include "DynamicMesh.h"

#include "Rendering/Renderer.h"


DynamicMesh::~DynamicMesh()
{
}

void DynamicMesh::Update(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    m_vertices = vertices;
    m_indices = indices;

    m_vertexBuffer->Bind();
    m_vertexBuffer->UpdateData(m_vertices);
    m_vertexBuffer->Unbind();
    
    m_indexBuffer->Bind();
    m_indexBuffer->UpdateData(m_indices);
    m_indexBuffer->Unbind();
    
}

