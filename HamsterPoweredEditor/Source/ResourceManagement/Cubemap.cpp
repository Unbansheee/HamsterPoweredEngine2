#include "Cubemap.h"

#include <iostream>

#include "Texture.h"
#include "stb_image/stb_image.h"

Cubemap::Cubemap(const std::string& path)
{
    glGenTextures(1, &id);
    Bind();
    this->path = path;
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);


    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        
        std::vector<unsigned char> right;
        std::vector<unsigned char> left;
        std::vector<unsigned char> top;
        std::vector<unsigned char> bottom;
        std::vector<unsigned char> front;
        std::vector<unsigned char> back;

        int faceSize = width / 4;
        unsigned int dataSize = width * height * nrChannels;
        
        right.reserve(faceSize * faceSize * nrChannels);
        left.reserve(faceSize * faceSize * nrChannels);
        top.reserve(faceSize * faceSize * nrChannels);
        bottom.reserve(faceSize * faceSize * nrChannels);
        front.reserve(faceSize * faceSize * nrChannels);
        back.reserve(faceSize * faceSize * nrChannels);
        
        GLint loadedcomponents = (nrChannels == 4) ? GL_RGBA : GL_RGB;

        for (int y = 0; y < faceSize; y++)
        {
            for (int x = 0; x < faceSize; x++)
            {
                for (int i = 0; i < nrChannels; i++)
                {
                    //get starting index of the pixel
                    unsigned int ylocationR = faceSize;
                    unsigned int xlocationR = faceSize * 2;
                    unsigned int indexR = (ylocationR + y) * width * nrChannels + (xlocationR + x) * nrChannels + i;
                    assert(indexR < dataSize, "Index out of bounds");
                    right.push_back(data[indexR]);

                    unsigned int ylocationL = faceSize;
                    unsigned int xlocationL = 0;
                    unsigned int indexL = ((ylocationL + y) * width + (xlocationL + x)) * nrChannels + i;
                    left.push_back(data[indexL]);

                    unsigned int ylocationT = 0;
                    unsigned int xlocationT = faceSize;
                    unsigned int indexT = ((ylocationT + y) * width + (xlocationT + x)) * nrChannels + i;
                    top.push_back(data[indexT]);

                    unsigned int ylocationB = faceSize * 2;
                    unsigned int xlocationB = faceSize;
                    unsigned int indexB = ((ylocationB + y) * width + (xlocationB + x)) * nrChannels + i;
                    bottom.push_back(data[indexB]);
                    

                    unsigned int ylocationF = faceSize;
                    unsigned int xlocationF = faceSize;
                    unsigned int indexF = ((ylocationF + y) * width + (xlocationF + x)) * nrChannels + i;
                    front.push_back(data[indexF]);

                    unsigned int ylocationBa = faceSize;
                    unsigned int xlocationBa = faceSize * 3;
                    unsigned int indexBa = ((ylocationBa + y) * width + (xlocationBa + x)) * nrChannels + i;
                    back.push_back(data[indexBa]);
                    
                    
                }
            }
        }
        

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, loadedcomponents, faceSize, faceSize, 0, loadedcomponents, GL_UNSIGNED_BYTE, right.data());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, loadedcomponents, faceSize, faceSize, 0, loadedcomponents, GL_UNSIGNED_BYTE, left.data());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, loadedcomponents, faceSize, faceSize, 0, loadedcomponents, GL_UNSIGNED_BYTE, top.data());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, loadedcomponents, faceSize, faceSize, 0, loadedcomponents, GL_UNSIGNED_BYTE, bottom.data());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, loadedcomponents, faceSize, faceSize, 0, loadedcomponents, GL_UNSIGNED_BYTE, front.data());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, loadedcomponents, faceSize, faceSize, 0, loadedcomponents, GL_UNSIGNED_BYTE, back.data());
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Cubemap texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }


    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    Unbind();

    stbi_set_flip_vertically_on_load(true);
    
    
}

Cubemap::~Cubemap()
{
    glDeleteTextures(1, &id);
    
}

void Cubemap::Bind(int slot)
{
    glBindTextureUnit(slot, id);
}

void Cubemap::Unbind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

