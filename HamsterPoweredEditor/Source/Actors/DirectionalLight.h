#pragma once
#include "Actor.h"

class DirLight : public Actor
{
public:
    void Update(Timestep ts) override;
    void Draw() override;
    void Begin() override;
    void OnDestroy() override;

    void SetColor(const glm::vec3& color);
    void SetIntensity(float intensity);
    glm::vec3 GetColor() const;
    float GetIntensity() const;
    
    nlohmann::json Serialize() override;
    void Deserialize(nlohmann::json& j) override;
protected:
    void OnInspectorGUI() override;
private:
    DirectionalLightData m_Light = { glm::vec3(0.0f, 1.f, 0.f), glm::vec3(1.0f), 1.0f };
    
    
};
