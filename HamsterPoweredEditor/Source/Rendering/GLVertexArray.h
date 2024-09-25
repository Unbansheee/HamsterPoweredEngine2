#pragma once
#include <memory>
#include <vector>
#include "Rendering/GLBuffers.h"

class GLVertexArray
{
public:
    GLVertexArray();
    virtual ~GLVertexArray();

    void Bind() const;
    void Unbind() const;

    virtual void AddVertexBuffer(const std::shared_ptr<GLVertexBuffer>& vertexBuffer);
    virtual void SetIndexBuffer(const std::shared_ptr<GLIndexBuffer>& indexBuffer);

    virtual const std::vector<std::shared_ptr<GLVertexBuffer>>& GetVertexBuffers() const {return m_VertexBuffers;}
    virtual const std::shared_ptr<GLIndexBuffer>& GetIndexBuffer() const {return m_IndexBuffer;}

private:
    std::vector<std::shared_ptr<GLVertexBuffer>> m_VertexBuffers;
    std::shared_ptr<GLIndexBuffer> m_IndexBuffer;
    uint32_t m_RendererID;
};
