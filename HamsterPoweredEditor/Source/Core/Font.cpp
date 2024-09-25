#include "Font.h"

#include <iostream>
#include <freetype/freetype.h>

#include "glad/glad.h"

Font* Font::LoadFont(const std::string& path, int size)
{

    if (fonts[{path, size}])
    {
        return fonts[{path, size}];
    }

    fonts[{path, size}] = new Font(path, size);
    return fonts[{path, size}];
}

Font::Font(const std::string& path, int size)
{

    if (FT_Init_FreeType(&m_ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
    
    if (FT_New_Face(m_ft, path.c_str(), 0, &m_face) != 0)
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }

    FT_Set_Pixel_Sizes(m_face, 0, size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    for (GLubyte Glyph = 0; Glyph < FontCharacterLimit; Glyph++)
    {
        if (FT_Load_Char(m_face, Glyph, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }
        
        
        FontChar FontCharacter = {
            Texture::CreateTexture(m_face->glyph->bitmap.buffer, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows, 1),
            glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
            (uint32_t)m_face->glyph->advance.x, glm::vec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows)
        };
        m_FontChars.insert(std::pair<char, FontChar>(Glyph, FontCharacter));
        m_path = path;
        m_size = size;

        std::string base_filename = path.substr(path.find_last_of("/\\") + 1);
        m_fontName = base_filename.substr(0, base_filename.find_last_of("."));

    
        
    }


    FT_Done_Face(m_face);
    FT_Done_FreeType(m_ft);
    std::cout << "Loaded font " << path << "   Size: " << m_size <<std::endl;

}
