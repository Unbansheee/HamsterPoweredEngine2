#include "View.h"

glm::vec3 View::GetForwardVector()
{
    return glm::normalize(glm::vec3{glm::sin(glm::radians(m_Rotation.y)) * glm::cos(glm::radians(m_Rotation.z)),
        glm::sin(glm::radians(m_Rotation.y)) * glm::sin(glm::radians(m_Rotation.z)),
        glm::cos(glm::radians(m_Rotation.y))});
}

glm::vec3 View::GetUpVector()
{
    return {glm::cross(GetRightVector(), GetForwardVector())};
}

glm::vec3 View::GetRightVector()
{
    glm::vec3 globalUp{0.0f,1.0f, 0.0f};
    return {glm::cross(GetForwardVector(), globalUp)};
}
