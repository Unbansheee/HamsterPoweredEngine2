#pragma once
#include "Actor.h"

class Spinner : public Actor
{
public:
    void Update(Timestep ts) override;
    void Draw() override;
    void Begin() override;
    void OnDestroy() override;
    nlohmann::json Serialize() override;
    void Deserialize(nlohmann::json& j) override;
protected:
    void OnInspectorGUI() override;
public:
    ~Spinner() override;
    void FixedUpdate(double ts) override;
    glm::vec3 rotationSpeed;
};
