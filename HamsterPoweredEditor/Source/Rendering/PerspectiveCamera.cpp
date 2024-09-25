#include "PerspectiveCamera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Rendering/Renderer.h"
PerspectiveCamera::PerspectiveCamera(float _fov, float _aspect, float _near, float _far) : fov(_fov), aspect(_aspect), near(_near), far(_far)
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
    m_ProjectionMatrix = glm::perspective(fov, aspect, near, far);
    m_ViewMatrix = glm::lookAt(m_Position, m_Position - cameraFront, GetUpVector());

    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; 
}

void PerspectiveCamera::SetProjection(float _fov, float _aspect, float _near, float _far)
{
    fov = _fov;
    aspect = _aspect;
    near = _near;
    far = _far;
    
    m_ProjectionMatrix = glm::perspective(fov, _aspect, _near, _far);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; 
}

void PerspectiveCamera::RecalculateViewMatrix()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
    
    m_ViewMatrix = glm::lookAt(m_Position, m_Position - cameraFront, GetUpVector());
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; 
}

