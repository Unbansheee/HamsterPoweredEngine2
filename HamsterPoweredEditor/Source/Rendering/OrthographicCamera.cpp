#include "OrthographicCamera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
{
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -100.f, 100.0f);
    m_ViewMatrix = glm::mat4(1.0f);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;  
}

void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
{
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -100.f, 100.0f);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; 
}

void OrthographicCamera::RecalculateViewMatrix()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
    glm::mat4 rotation = glm::mat4(1.f);
    rotation = glm::rotate(rotation, m_Rotation.x, glm::vec3(1.f, 0.f, 0.f));
    rotation = glm::rotate(rotation, m_Rotation.y, glm::vec3(0.f, 1.f, 0.f));
    rotation = glm::rotate(rotation, m_Rotation.z, glm::vec3(0.f, 0.f, 1.f));
    
    transform = transform * rotation;

    m_ViewMatrix = glm::inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

