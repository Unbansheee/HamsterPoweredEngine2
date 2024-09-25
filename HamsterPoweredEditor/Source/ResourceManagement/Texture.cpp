#include "Texture.h"

#include <iostream>

#include "Rendering/Renderer.h"
#include "stb_image/stb_image.h"

void Texture::SetFilteringMode(FilteringMode mode)
{
    glBindTexture(GL_TEXTURE_2D, rendererID);
    filteringMode = mode;
    switch (mode)
    {
    case FilteringMode::NEAREST:
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    case FilteringMode::LINEAR:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    }
    Unbind();
        
}

Texture::FilteringMode Texture::GetFilteringMode() const
{
    return filteringMode;
}

Texture* Texture::CreateTexture(const std::string& path, GLenum target, FilteringMode mode)
{
    if (path.empty())
    {
        std::cout << "Texture path is empty" << std::endl;
        return nullptr;
    }
    if (m_textureCache[path])
    {
        return m_textureCache[path];
    }
    m_textureCache[path] = new Texture(path, target);
    return m_textureCache[path];
}

Texture* Texture::CreateTextureUncached(const std::string& path, FilteringMode mode)
{
    return new Texture(path, GL_TEXTURE_2D);
}

Texture* Texture::CreateTexture(unsigned char* data, int _width, int _height, int channels, FilteringMode mode)
{
    Texture* texture = new Texture(data, _width, _height, channels, mode);
    return texture;
}

Texture* Texture::GetWhiteTexture()
{
    if (m_whiteTexture == nullptr)
    {
        unsigned char data[4] = { 255, 255, 255, 255 };
        m_whiteTexture = CreateTexture(data, 1, 1, 4);
    }
    return m_whiteTexture;
    
}

void Texture::DeleteTextureID(uint32_t id)
{
    glDeleteTextures(1, &id);
  
    for (auto it = m_textureCache.begin(); it != m_textureCache.end(); it++)
    {
        if (it->second->rendererID == id)
        {
            DeleteTexture(it->second);
            m_textureCache.erase(it);
            break;
        }
    }
}

void Texture::DeleteTexture(Texture* texture)
{
    delete texture;
    texture = nullptr;
}

Texture::Texture(const std::string& path, GLenum target, FilteringMode mode)
    : rendererID(0), filePath(path), localBuffer(NULL), width(0), height(0), bpp(0)
{

    
    stbi_set_flip_vertically_on_load(1);
    localBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 0);
    if (localBuffer == NULL)
    {
        std::cout << "Failed to load texture: " << path << std::endl;
    }

    GLenum internalFormat = 0, dataFormat = 0;
    if (bpp == 1)
    {
        internalFormat = GL_RED;
        dataFormat = GL_RED;
    }
    else if (bpp == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }
    else if (bpp == 4)
    {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    else
    {
        std::cout << "Image format not supported" << std::endl;
    }
    
    
    
    glCreateTextures(target, 1, &rendererID);
    glTextureStorage2D(rendererID, 1, internalFormat, width, height);

    SetFilteringMode(mode);
    glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTextureSubImage2D(rendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, localBuffer);

    if (localBuffer)
        stbi_image_free(localBuffer);
    
}

Texture::Texture(unsigned char* data, int width, int height, int bpp, FilteringMode mode)
    : localBuffer(nullptr), width(width), height(height), bpp(bpp)
{
    GLenum internalFormat = 0, dataFormat = 0;
    if (bpp == 1)
    {
        internalFormat = GL_RED;
        dataFormat = GL_RED;
    }
    else if (bpp == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }
    else if (bpp == 4)
    {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    else
    {
        std::cout << "Image format not supported" << std::endl;
    }
    
    glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
    glBindTexture(GL_TEXTURE_2D, rendererID);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

    SetFilteringMode(mode);
    glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
}

Texture::~Texture()
{
    glDeleteTextures(1, &rendererID);
}

void Texture::Bind(unsigned slot) const
{
    glActiveTexture(slot);
    glBindTextureUnit(slot, rendererID);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
