#include "GLFrameBuffer.h"
#include "GLRenderBuffer.h"

#include <iostream>
#include <glad/glad.h>

GLFrameBuffer::GLFrameBuffer(const FrameBufferSpecification& spec) : m_Specification(spec)
{
    Invalidate();
}

GLFrameBuffer::~GLFrameBuffer()
{
    glDeleteFramebuffers(1, &m_RenderBufferID);
    glDeleteFramebuffers(1, &m_FinalBufferID);
    glDeleteTextures(1, &m_FinalColourTextureID);
}

void GLFrameBuffer::BindMSAABuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_RenderBufferID);
    glViewport(0, 0, m_Specification.Width, m_Specification.Height);
}

void GLFrameBuffer::BindFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FinalBufferID);
    glViewport(0, 0, m_Specification.Width, m_Specification.Height);
}

void GLFrameBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFrameBuffer::BlitToScreen()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RenderBufferID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FinalBufferID);
    glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0, m_Specification.Width, m_Specification.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFrameBuffer::Invalidate()
{
    bool frame1valid = false;
    bool frame2valid = false;
    if (m_RenderBufferID)
    {
        glDeleteFramebuffers(1, &m_RenderBufferID);
        glDeleteFramebuffers(1, &m_FinalBufferID);
        glDeleteTextures(1, &m_FinalColourTextureID);
    }

    ///////////////
    //MSAA BUFFER
    //////////////
    glCreateFramebuffers(1, &m_RenderBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RenderBufferID);

    //Create MSAA render buffers
    m_MSAAColorBuffer.reset(new GLRenderBuffer(m_Specification.Width, m_Specification.Height, m_Specification.Samples, GL_RGBA8));
    m_MSAADepthBuffer.reset(new GLRenderBuffer(m_Specification.Width, m_Specification.Height, m_Specification.Samples, GL_DEPTH_COMPONENT24));

    //Attach buffers
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_MSAAColorBuffer->GetID());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_MSAADepthBuffer->GetID());

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    } else frame1valid = true;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    ////////////////////
    //FINAL IMAGE BUFFER
    ////////////////////
    
    //Set up final image texture
    glGenTextures(1, &m_FinalColourTextureID);
    glBindTexture(GL_TEXTURE_2D, m_FinalColourTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //Create buffer
    glGenFramebuffers(1, &m_FinalBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FinalBufferID);

    //Attach final depth and colour buffer to framebuffer
    m_FinalDepthTextureID.reset(new GLRenderBuffer(m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_COMPONENT24));
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FinalColourTextureID, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_FinalDepthTextureID->GetID());

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    } else frame2valid = true;
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    m_Valid = frame1valid && frame2valid;
    
}

void GLFrameBuffer::Resize(uint32_t width, uint32_t height)
{
    m_Specification.Width = width;
    m_Specification.Height = height;
    Invalidate();
}
