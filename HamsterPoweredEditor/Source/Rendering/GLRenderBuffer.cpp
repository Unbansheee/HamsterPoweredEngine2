#include "GLRenderBuffer.h"
#include <glad/glad.h>


GLRenderBuffer::GLRenderBuffer(int width, int height, int samples, GLenum internalFormat) : m_width(width), m_height(height), m_samples(samples), m_internalFormat(internalFormat)
{
    Create(width, height, samples, internalFormat);
}

GLRenderBuffer::~GLRenderBuffer()
{
    glDeleteRenderbuffers(1, &m_renderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

bool GLRenderBuffer::Create(int width, int height, int samples, GLenum internalFormat)
{
    m_width = width;
    m_height = height;
    m_samples = samples;
    m_internalFormat = internalFormat;
    glDeleteRenderbuffers(1, &m_renderBufferId);
    
    glGenRenderbuffers(1, &m_renderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    return true;
}

void GLRenderBuffer::Bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId);
}

void GLRenderBuffer::Unbind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}