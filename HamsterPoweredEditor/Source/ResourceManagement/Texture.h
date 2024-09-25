#pragma once
#include "Rendering/Renderer.h"

class Texture
{
private:

    std::string filePath;
    unsigned char* localBuffer;
    int width, height, bpp;

public:
    enum FilteringMode
    {
        NEAREST,
        LINEAR
    };

    enum Type
    {
        DEFAULT,
        CUBEMAP
    };
    
    unsigned int rendererID;
    
    std::string GetPath() const { return filePath; }

    void Bind(unsigned int slot = 0) const;
    void Unbind();

    inline int GetWidth() const {return width;}
    inline int GetHeight() const {return height;}

    void SetFilteringMode(FilteringMode mode);
    FilteringMode GetFilteringMode() const;

    //Factory function to ensure each texture only gets loaded once. Stored in a cache and will return a pointer to an already existing texture if the same path is used.
    //Constructor is private to ensure only the cache is used.
    //Currently there is no way to unload a texture from the cache, causing a memory leak. Reference counting would be a potential solution.
    static Texture* CreateTexture(const std::string& path, GLenum target = GL_TEXTURE_2D, FilteringMode mode = LINEAR);

    //Alternative. This is not cached and the user is responsible for deleting the texture.
    static Texture* CreateTextureUncached(const std::string& path, FilteringMode mode = LINEAR);

    //This is not managed by the cache and the user is responsible for deleting the texture.
    static Texture* CreateTexture(unsigned char* data, int _width, int _height, int channels, FilteringMode mode = LINEAR);

    static Texture* GetWhiteTexture();

    static void DeleteTextureID(uint32_t id);
    static void DeleteTexture(Texture* texture);
private:
    FilteringMode filteringMode = LINEAR;
    inline static std::unordered_map<std::string, Texture*> m_textureCache;
    inline static Texture* m_whiteTexture = nullptr;
    Texture(const std::string& path, GLenum target, FilteringMode mode = LINEAR);
    Texture(unsigned char* data, int width, int height, int bpp = 4, FilteringMode mode = LINEAR);
    ~Texture();
};



 