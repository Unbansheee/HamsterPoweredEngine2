#pragma once
#include <glm/fwd.hpp>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>

class View
{
public:
    virtual ~View() = default;
    const glm::vec3& GetPosition() const { return m_Position; }
    const glm::vec3& GetRotation() const { return m_Rotation; }
    void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix();}
    void SetRotation(glm::vec3 rotation)
    {
        m_Rotation = rotation; RecalculateViewMatrix();
    }

    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    glm::vec3 GetForwardVector();
    glm::vec3 GetUpVector();
    glm::vec3 GetRightVector();
    
protected:
    virtual void RecalculateViewMatrix() = 0;
    
protected:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_Position = {0, 0, 0};
    glm::vec3 m_Rotation = {0, 0, 0};
};
