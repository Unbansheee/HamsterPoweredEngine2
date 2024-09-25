#pragma once
#include "Actor.h"
#include "Rendering/OrthographicCamera.h"

//Perspective camera rotation is currently broken
class CameraController : public Actor
{
public:
    enum class CameraType
    {
        ORTHO,
        PERSPECTIVE
    };
    
public:
    CameraController(CameraType type, float aspectRatio);
    void SetCameraType(CameraType type);
    void HandleMouseMovement(float x, float y);
    void Update(Timestep ts) override;
    void Begin() override;
    void OnDestroy() override;
    float GetZoom() const { return m_CameraType == CameraType::ORTHO ? m_OrthoZoom : m_Zoom; }
    void SetZoom(float zoom);
    void Move(glm::vec3 offset);
    CameraType GetCameraType() const { return m_CameraType; }
    float GetPerspSpeed() const { return m_PerspMoveSpeed; }
    void SetPerspSpeed(float speed) { m_PerspMoveSpeed = glm::clamp(speed, 0.1f, 20.f); }

    void Resize(float width, float height);
    View* GetCamera() { return m_Camera; }
    const View* GetCamera() const { return m_Camera; }

protected:
    void OnInspectorGUI() override;
public:
    nlohmann::json Serialize() override;
    void Deserialize(nlohmann::json& j) override;
    void FixedUpdate(double ts) override;

private:
    float yaw = 0.0f;
    float pitch = 0.0f;
    CameraType m_CameraType;
    float dt = 0;
    float m_Zoom = 75.f;
    float m_OrthoZoom = 2.5f;
    float m_AspectRatio;
    float m_PerspMoveSpeed = 1.f;
    View* m_Camera;
};
