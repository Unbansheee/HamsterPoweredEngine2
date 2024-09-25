#pragma once
#include "Actor.h"
#include "Rendering/StaticMesh.h"
#include "ResourceManagement/Cubemap.h"

class SkyboxActor : public Actor
{
    
public:
    SkyboxActor();
    
    void FixedUpdate(double ts) override;
    void Update(Timestep ts) override;
    void Draw() override;
    void Begin() override;
    void OnDestroy() override;
    nlohmann::json Serialize() override;
    void Deserialize(nlohmann::json& j) override;
    
    
    
protected:
    void OnInspectorGUI() override;

public:
    ~SkyboxActor() override;
    std::string path = "Resources/Textures/Skybox/Hilltop.png";
    StaticMesh mesh;
    std::shared_ptr<Cubemap> cubemap;
    float brightness = 1.0f;
};
