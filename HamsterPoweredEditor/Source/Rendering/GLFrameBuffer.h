#pragma once
#include <cstdint>
#include <memory>
#include <glm/vec2.hpp>

#include "GLRenderBuffer.h"

struct FrameBufferSpecification
{
    uint32_t Width, Height;
    uint32_t Samples = 4;
    bool SwapChainTarget = false;
};

class GLFrameBuffer
{
public:
    virtual const FrameBufferSpecification& GetSpecification() const {return m_Specification;}
    
    GLFrameBuffer(const FrameBufferSpecification& spec);
    virtual ~GLFrameBuffer();

    virtual void BindMSAABuffer();
    virtual void BindFrameBuffer();
    virtual void Unbind();
    virtual void BlitToScreen();

    bool IsValid() const {return m_Valid;}

    uint32_t GetFinalFrameColorID() const {return m_FinalColourTextureID;}
    
    void Invalidate();
    void Resize(uint32_t width, uint32_t height);

    uint32_t m_RenderBufferID = 0;
    uint32_t m_FinalBufferID = 0;
private:
    FrameBufferSpecification m_Specification;

    uint32_t m_FinalColourTextureID = 0;
    std::shared_ptr<GLRenderBuffer> m_FinalDepthTextureID;
    
    std::shared_ptr<GLRenderBuffer> m_MSAADepthBuffer;
    std::shared_ptr<GLRenderBuffer> m_MSAAColorBuffer;

    bool m_Valid = false;
    
};
