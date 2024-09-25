#pragma once
#include "Actor.h"
#include "DynamicMeshActor.h"
#include "PerlinNoise.hpp"
#include "Core/Raycast.h"
#include "Rendering/DynamicMesh.h"

class MeshActor;

struct ClothParticle
{
    glm::vec3 Position = {0, 0, 0};
    glm::vec3 OldPosition = {0, 0, 0};
    glm::vec3 Velocity = { 0, 0, 0 };
    glm::vec3 Acceleration = {0, 0, 0};
    glm::vec3 Force = { 0, 0, 0 };
    float Mass = 1.f;

    bool bFixed;
    bool bCollisionParticle;
};

struct ClothConstraint
{
    ClothConstraint(ClothParticle* p1, ClothParticle* p2, float restLength)
        : P1(p1), P2(p2), RestLength(restLength) {}

    ClothParticle* P1;
    ClothParticle* P2;
    float RestLength = 1.f;
    
};

class ClothActor : public DynamicMeshActor
{
public:
    void FixedUpdate(double ts) override;
    void Update(Timestep ts) override;
    void Draw() override;
    void Begin() override;
    void OnDestroy() override;
    nlohmann::json Serialize() override;
    void Deserialize(nlohmann::json& j) override;

protected:
    // Editor widget
    void OnInspectorGUI() override;

    // Reset cloth particles, constraints and mesh to defaults
    void ResetCloth();

    // Generate particles and constraints
    void GenerateCloth(int width, int height);

    // Make the forces go wee woo
    void VerletIntegration(float deltaTime);

    // Fix all the mistakes the forces made >:(
    void ResolveCollisions(float ts);

    // Prevent particles from being a not cloth
    void SatisfyConstraints(int iterations, float ts);

    // Fills in the inputs with vertices and indices derived from particles
    void GetVertices(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

    // Shortcut to update the mesh
    void UpdateMesh();

    int width = 10; // Number of particles in the x direction
    int height = 10; // Number of particles in the y direction
    float restLength = 1.f; // Distance between particles
    float stiffness = 0.1f; // How much the constraints resist being stretched
    float damping = 0.1f; // Slow down over time
    int iterations = 4; // Number of times to satisfy constraints
    float physicsTime = 200.f; // DEPRECATED: How long to run physics for
    bool collisionEnabled = true; // Whether to resolve collisions against sphere
    float windTurbulenceStrength = 10.f; // How much the wind turbulence affects the cloth
    float windFrequency = 0.5f; // How often the wind turbulence changes
    siv::PerlinNoise perlin; // Perlin noise generator for wind turbulence
    float curtainRailDistance = 1.f; // 0-1 How far the curtain rail is from the origin
    bool selfCollisionEnabled = false; // Whether to resolve collisions against other particles
    
    
    StaticMesh m_collisionMesh;
    StaticMesh m_hook;

    // Keeping track of collision sphere info
    glm::vec3 m_collisionMeshCurrentPosition = { 0, 0, 0 };
    float m_collisionMeshScale = 1.f;
    glm::vec3 m_collisionMeshOrigin = { 0, 0, 0 };
    float m_collisionMeshSpeed = 1.f;
    glm::vec3 m_collisionTravelDistance = {0, 0, 1};

    //Forces
    glm::vec3 m_gravity = { 0, -9.81f, 0 };
    glm::vec3 m_wind = { 0, 0, 0 };
    
    std::vector<ClothParticle> m_particles;
    std::vector<ClothConstraint> m_constraints;
    std::vector<Vertex> m_verts;
    std::vector<uint32_t> m_indices;
    bool m_mouseDown;
    Hit m_mouseHit;
    ClothParticle* m_mouseDownVert;

public:
    ~ClothActor() override;
    
};
