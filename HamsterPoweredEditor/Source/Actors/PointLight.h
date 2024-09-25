#pragma once
#include "Actor.h"

class PointLight : public Actor
{
public:
    void Update(Timestep ts) override;
    void Draw() override;
    void Begin() override;
    void OnDestroy() override;
    nlohmann::json Serialize() override;
    void Deserialize(nlohmann::json& j) override;

    void SetColor(const glm::vec3& color);
    void SetIntensity(float intensity);
    void SetRadius(float radius);

    glm::vec3 GetColor() const;
    float GetIntensity() const;
    float GetRadius() const;
    
    
protected:
    void OnInspectorGUI() override;

    PointLightData m_Light = {{0, 0, 0}, {1, 1, 1}, 1.0f, 0.1f};
public:
    
};
