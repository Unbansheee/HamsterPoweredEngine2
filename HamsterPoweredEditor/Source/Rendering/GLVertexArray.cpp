#include "GLVertexArray.h"
#include "GLBuffers.h"
#include "VertexBufferLayout.h"
#include "glad/glad.h"

GLVertexArray::GLVertexArray()
{
    glCreateVertexArrays(1, &m_RendererID);
    
}

GLVertexArray::~GLVertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}


void GLVertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void GLVertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void GLVertexArray::AddVertexBuffer(const std::shared_ptr<GLVertexBuffer>& vertexBuffer)
{
    glBindVertexArray(m_RendererID);
    vertexBuffer->Bind();
    
    uint32_t index = 0;
    const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : layout)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, (GLint)element.GetComponentCount(), ShaderDataTypeToGL(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, (GLint)layout.GetStride(), (const void*)(uint64_t)element.Offset);
        index++;
    }

    m_VertexBuffers.push_back(vertexBuffer);
    
}

void GLVertexArray::SetIndexBuffer(const std::shared_ptr<GLIndexBuffer>& indexBuffer)
{
    glBindVertexArray(m_RendererID);
    indexBuffer->Bind();

    m_IndexBuffer = indexBuffer;
}
