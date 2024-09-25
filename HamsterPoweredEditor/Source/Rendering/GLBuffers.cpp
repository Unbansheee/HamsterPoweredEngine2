#include "GLBuffers.h"

#include <glad/glad.h>

/////////////////////////////
// Vertex Buffer
/////////////////////////////
GLVertexBuffer::GLVertexBuffer(float* vertices, uint32_t size, uint32_t drawMode)
{
    m_DrawMode = drawMode;
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, m_DrawMode);
}

GLVertexBuffer::GLVertexBuffer(const std::vector<Vertex>& vertices, uint32_t drawMode)
{
    m_DrawMode = drawMode;
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), m_DrawMode);
}

GLVertexBuffer::~GLVertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void GLVertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void GLVertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
}

void GLVertexBuffer::UpdateData(float* vertices, uint32_t size, uint32_t offset)
{
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, vertices);
}

void GLVertexBuffer::ClearData() const
{
    glClearBufferData(GL_ARRAY_BUFFER, GL_R32F, GL_RED, GL_FLOAT, NULL);
    
}


void GLVertexBuffer::UpdateData(std::vector<Vertex>& vertices, uint32_t offset)
{
    glBufferSubData(GL_ARRAY_BUFFER, offset, vertices.size() * sizeof(Vertex), vertices.data());
}


/////////////////////////////
//Index Buffer
/////////////////////////////
GLIndexBuffer::GLIndexBuffer(uint32_t* indices, uint32_t count, uint32_t drawMode) : m_Count(count), m_DrawMode(drawMode)
{
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, m_DrawMode);
}

GLIndexBuffer::GLIndexBuffer(const std::vector<uint32_t>& indices, uint32_t drawMode) : m_Count(indices.size()), m_DrawMode(drawMode)
{
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), m_DrawMode);
}

GLIndexBuffer::~GLIndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
    
}

void GLIndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    
}

void GLIndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
}

void GLIndexBuffer::UpdateData(uint32_t* indices, uint32_t count, uint32_t offset) 
{
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, count * sizeof(uint32_t), indices);
    m_Count = (offset / sizeof(uint32_t)) + count;
}

void GLIndexBuffer::UpdateData(const std::vector<uint32_t>& indices, uint32_t offset)
{
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, indices.size() * sizeof(uint32_t), indices.data());
    m_Count = (offset / sizeof(uint32_t)) + indices.size();
}
