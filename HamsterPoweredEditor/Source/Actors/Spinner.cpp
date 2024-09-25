#include "Spinner.h"

void Spinner::Update(Timestep ts)
{
    Actor::Update(ts);
    SetRotation(GetRotation() + ts.GetSeconds() * rotationSpeed);
}

void Spinner::Draw()
{
    Actor::Draw();
}

void Spinner::Begin()
{
    Actor::Begin();
    SetRotation(0, 0, 0);
}

void Spinner::OnDestroy()
{
    Actor::OnDestroy();
}

nlohmann::json Spinner::Serialize()
{
    auto j = Actor::Serialize();
    j["RotationSpeed"] = rotationSpeed;
    return j;
    
}

void Spinner::Deserialize(nlohmann::json& j)
{
    Actor::Deserialize(j);
    rotationSpeed = j["RotationSpeed"];
}

void Spinner::OnInspectorGUI()
{
    Actor::OnInspectorGUI();
    ImGui::DragFloat3("Rotation##Spinner", &rotationSpeed.x, 0.1f);
}

Spinner::~Spinner()
{
}

void Spinner::FixedUpdate(double ts)
{
}
