#pragma once
#include <glm/vec3.hpp>

struct PointLightData
{
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float radius;
};

struct DirectionalLightData
{
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
};