#include "CameraController.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "imgui.h"
#include "Core/App.h"
#include "Core/Input.h"
#include "Core/Window.h"
#include "Rendering/PerspectiveCamera.h"


CameraController::CameraController(CameraType type, float aspectRatio) : m_CameraType(type), m_AspectRatio(aspectRatio)
    
{
    if (type == CameraType::ORTHO)
    {
        m_Camera = new OrthographicCamera(-m_AspectRatio * m_OrthoZoom, m_AspectRatio * m_OrthoZoom, -m_OrthoZoom, m_OrthoZoom);
    }
    else
    {
        m_Camera = new PerspectiveCamera(glm::radians(m_Zoom), m_AspectRatio, 1.f, 10000.0f);
    }
    
}

void CameraController::SetCameraType(CameraType type)
{
    if (type == m_CameraType)
        return;
    delete m_Camera;
    m_CameraType = type;
    if (type == CameraType::ORTHO)
    {
        m_Camera = new OrthographicCamera(-m_AspectRatio * m_OrthoZoom, m_AspectRatio * m_OrthoZoom, -m_OrthoZoom, m_OrthoZoom);
        SetRotation(0.f, 0.f, 0.f);
        SetZoom(GetZoom());
    }
    else
    {
        m_Camera = new PerspectiveCamera(glm::radians(m_Zoom), m_AspectRatio, 1.f, 10000.0f);
        SetRotation(0.f, 0.f, 0.f);
        
        
    }
    
}

void CameraController::HandleMouseMovement(float x, float y)
{
    if (abs(x) > 0 || abs(y) > 0)
    {
        if (m_CameraType == CameraType::ORTHO)
        {
            SetPosition(GetPosition() + glm::vec3(x * 0.0028f * m_OrthoZoom, y * 0.0028f * m_OrthoZoom, 0.f));
        }
        else
        {
            auto camera = dynamic_cast<PerspectiveCamera*>(m_Camera);
            camera->pitch += y * 0.5f;
            camera->yaw -= x * 0.5f;

            if(camera->pitch > 89.0f)
                camera->pitch = 89.0f;
            if(camera->pitch < -89.0f)
                camera->pitch = -89.0f;

            yaw = camera->yaw;
            pitch = camera->pitch;
            
            camera->RecalculateViewMatrix();

        }
    }
}

void CameraController::Update(Timestep ts)
{
    dt =  ts.GetSeconds();
    m_Camera->SetPosition(GetPosition());
    m_Camera->SetRotation(GetRotation());

    if (Input::WasKeyPressed(Keyboard::F1))
    {
        Renderer::SetRenderMode(Renderer::RenderMode::LIT);
    }
    if (Input::WasKeyPressed(Keyboard::F2))
    {
        Renderer::SetRenderMode(Renderer::RenderMode::WIREFRAME);
    }
    if (Input::WasKeyPressed(Keyboard::F3))
    {
        Renderer::SetRenderMode(Renderer::RenderMode::UNLIT);
    }

    if (Input::WasKeyPressed(Keyboard::F4))
    {
        SetCameraType(CameraType::ORTHO);
    }
    if (Input::WasKeyPressed(Keyboard::F5))
    {
        SetCameraType(CameraType::PERSPECTIVE);
    }
    
    

}

void CameraController::Begin()
{
}

void CameraController::OnDestroy()
{
}

void CameraController::SetZoom(float zoom)
{
    
    if (m_CameraType == CameraType::ORTHO)
    {
        m_OrthoZoom = glm::clamp(zoom, 0.1f, 100.f);
        static_cast<OrthographicCamera*>(m_Camera)->SetProjection(-m_AspectRatio * m_OrthoZoom, m_AspectRatio * m_OrthoZoom, -m_OrthoZoom, m_OrthoZoom);
    }
    else
    {
        m_Zoom = glm::clamp(zoom, 0.1f, 140.f);
        static_cast<PerspectiveCamera*>(m_Camera)->SetProjection(glm::radians(m_Zoom), m_AspectRatio, 1.f, 10000.0f);
    }
}

void CameraController::Move(glm::vec3 direction)
{
    if (m_CameraType == CameraType::ORTHO)
    {
        return;
    }
    else
    {
        auto cam = dynamic_cast<PerspectiveCamera*>(m_Camera);
        glm::vec3 forwardMotion = cam->cameraFront * dt * -direction.x;
        glm::vec3 rightMotion = glm::cross(cam->cameraFront, {0, 1, 0}) * dt * -direction.y;
        glm::vec3 upMotion = m_Camera->GetUpVector() * dt * -direction.z;
        SetPosition(GetPosition() + ((forwardMotion + rightMotion + upMotion) * m_PerspMoveSpeed));
    }
    
}

void CameraController::Resize(float width, float height)
{
    m_AspectRatio = width / height;
    if (m_CameraType == CameraType::ORTHO)
    {
        static_cast<OrthographicCamera*>(m_Camera)->SetProjection(-m_AspectRatio * m_OrthoZoom, m_AspectRatio * m_OrthoZoom, -m_OrthoZoom, m_OrthoZoom);
    }
    else
    {
        static_cast<PerspectiveCamera*>(m_Camera)->SetProjection(glm::radians(m_Zoom), m_AspectRatio, 1.f, 10000.0f);
    }
}

void CameraController::OnInspectorGUI()
{
    if (ImGui::CollapsingHeader("Camera"))
    {
        if (ImGui::RadioButton("Perspective", m_CameraType == CameraType::PERSPECTIVE))
        {
            SetCameraType(CameraType::PERSPECTIVE);
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Orthographic", m_CameraType == CameraType::ORTHO))
        {
            SetCameraType(CameraType::ORTHO);
        }
        switch (m_CameraType)
        {
            case CameraType::PERSPECTIVE:
            {
                float z = m_Zoom;
                if (ImGui::DragFloat("FOV", &z, 0.1f, 0.1f, 140.f))
                {
                    SetZoom(z);
                }

                if (ImGui::DragFloat("Move Speed", &m_PerspMoveSpeed, 0.1f, 0.1f, 20.f))
                {
                }

                if (ImGui::DragFloat("Yaw", &yaw, 1.f))
                {
                    auto camera = dynamic_cast<PerspectiveCamera*>(m_Camera);
                    camera->yaw = yaw;
                    camera->RecalculateViewMatrix();
                }

                if (ImGui::DragFloat("Pitch", &pitch, 0.1f, -89.f, 89.f))
                {
                    auto camera = dynamic_cast<PerspectiveCamera*>(m_Camera);
                    camera->pitch = pitch;
                    camera->RecalculateViewMatrix();
                }
                break;
            }
            case CameraType::ORTHO:
            {
                    float z = m_OrthoZoom;
                    if (ImGui::DragFloat("Zoom", &z, 0.1f, 0.1f, 100.f))
                    {
                        SetZoom(z);
                    }
                break;
            }
        }

        ImGui::Separator();
    }
    
}

nlohmann::json CameraController::Serialize()
{
    nlohmann::json j =  Actor::Serialize();
    j["CameraType"] = m_CameraType;
    j["Zoom"] = m_Zoom;
    j["OrthoZoom"] = m_OrthoZoom;
    j["PerspMoveSpeed"] = m_PerspMoveSpeed;
    j["Yaw"] = yaw;
    j["Pitch"] = pitch;
    return j;
}

void CameraController::Deserialize(nlohmann::json& j)
{
    Actor::Deserialize(j);
    m_Zoom = j["Zoom"];
    m_OrthoZoom = j["OrthoZoom"];
    m_PerspMoveSpeed = j["PerspMoveSpeed"];

    SetCameraType(j["CameraType"]);
    if (m_CameraType == CameraType::ORTHO)
    {
        SetZoom(m_OrthoZoom);
    }
    else
    {
        SetZoom(m_Zoom);
        dynamic_cast<PerspectiveCamera*>(m_Camera)->yaw = j["Yaw"];
        dynamic_cast<PerspectiveCamera*>(m_Camera)->pitch = j["Pitch"];
        yaw = j["Yaw"];
        pitch = j["Pitch"];
    }
    Resize(Renderer::GetViewportSize().x, Renderer::GetViewportSize().y);
}

void CameraController::FixedUpdate(double ts)
{
}

