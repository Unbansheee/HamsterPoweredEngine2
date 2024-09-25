#include "DirectionalLight.h"

void DirLight::Update(Timestep ts)
{
    Actor::Update(ts);
    glm::vec3 forwardVector;
    //convert m_Transform to a forward vector
    forwardVector.x = m_transform[0][2];
    forwardVector.y = m_transform[1][2];
    forwardVector.z = m_transform[2][2];
    //set the direction of the light to the forward vector
    m_Light.direction = forwardVector;
    
}

void DirLight::Draw()
{
    Actor::Draw();
    Renderer::Submit(m_Light);
}

void DirLight::Begin()
{
    Actor::Begin();
}

void DirLight::OnDestroy()
{
    Actor::OnDestroy();
}

void DirLight::SetColor(const glm::vec3& color)
{
    m_Light.color = color;
}

void DirLight::SetIntensity(float intensity)
{
    m_Light.intensity = intensity;
}

glm::vec3 DirLight::GetColor() const
{
    return m_Light.color;
}

float DirLight::GetIntensity() const
{
    return m_Light.intensity;
}

nlohmann::json DirLight::Serialize()
{
    auto j = Actor::Serialize();
    j["Intensity"] = m_Light.intensity;
    j["Color"] = m_Light.color;
    return j;
}

void DirLight::Deserialize(nlohmann::json& j)
{
    Actor::Deserialize(j);
    m_Light.intensity = j["Intensity"];
    m_Light.color = j["Color"];
}

void DirLight::OnInspectorGUI()
{
    Actor::OnInspectorGUI();
    ImGui::ColorEdit3("Color", &m_Light.color[0]);
    ImGui::DragFloat("Intensity", &m_Light.intensity, 0.1f, 0.0f, 100.0f);
}
