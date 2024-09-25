#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex
{
    Vertex(glm::vec3 pos, glm::vec3 colour, glm::vec2 texcoord, glm::vec3 normal, int ind) : Position(pos), Colour(colour), TexCoord(texcoord), Normal(normal), TexIndex(ind) {}
    Vertex() {};
    
    glm::vec3 Position = {0, 0, 0};
    glm::vec3 Colour = {0, 0, 0};
    glm::vec2 TexCoord = {0, 0};
    glm::vec3 Normal = {0, 0, 0};
    int TexIndex = 0;
};
