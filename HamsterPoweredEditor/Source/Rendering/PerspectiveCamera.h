#pragma once
#include "Renderer.h"
#include "View.h"

class PerspectiveCamera : public View
{
public:
    PerspectiveCamera(float _fov = glm::radians(45.f), float _aspect = (float)Renderer::AspectRatio(), float _near = 0.1f, float _far = 10000.f);
    void SetProjection(float _fov, float _aspect, float _near, float _far);

    void RecalculateViewMatrix() override;

    glm::vec3 cameraFront;
    float yaw = 0.f;
    float pitch = 90.f;

private:

    float fov = 45.0f;
    float aspect = 1.f;
    float near = 0.1f;
    float far = 100.0f;


};
