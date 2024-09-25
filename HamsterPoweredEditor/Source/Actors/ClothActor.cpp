#include "ClothActor.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "MeshActor.h"
#include "Core/Input.h"
#include "Core/Scene.h"
#include "PerlinNoise.hpp"

glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t)
{
    return a + t * (b - a);
}

void ClothActor::FixedUpdate(double ts)
{
    
    VerletIntegration(ts);
    SatisfyConstraints(iterations, ts);
    
}

void ClothActor::Update(Timestep ts)
{

    m_collisionMeshCurrentPosition = lerp(m_collisionMeshOrigin + m_collisionTravelDistance/2.f, m_collisionMeshOrigin - m_collisionTravelDistance/2.f, (float)sin(ts.GetTimeAsSeconds() * m_collisionMeshSpeed));
    if (collisionEnabled) ResolveCollisions(ts.GetSeconds());

    if (Input::WasKeyPressed(Keyboard::Space))
    {
        GenerateCloth(width, height);
    }
    
    //If mouse is clicked, raycast to find the nearest vertice and attach it to the mouse
    if (Input::WasMouseButtonPressed(Mouse::Left))
    {
        auto hit = Input::RaycastMouse();
        if (hit.Actor == this)
        {\
                m_mouseDown = true;
                m_mouseHit = hit;

                // Find closest point
                float minDist = 1000000.f;
                for (auto& p : m_particles)
                {
                    float dist = glm::distance(p.Position, m_mouseHit.Location);
                    if (dist < minDist)
                    {
                        minDist = dist;
                        m_mouseDownVert = &p;
                    }
                }
            
        }
    }
    

    // Let go of the dragged vertice if mouse is released (if there is one)
    if (Input::WasMouseButtonReleased(Mouse::Left))
    {
        m_mouseDown = false;
        if (m_mouseDownVert)
        {
            m_mouseDownVert->bFixed = false;
            m_mouseDownVert = nullptr;
        }
    }


    // If we currently have a vertex selected and are holding mouse button, make it follow the mouse in projected screenspace
    if (m_mouseDown)
    {
        if (m_mouseDownVert)
        {
            // translate vert perpendicular to the hit normal
            glm::vec3 v0 = Renderer::ScreenToWorldPos(Input::GetMousePositionInViewport(), 0.0f);
            glm::vec3 v1 = Renderer::ScreenToWorldPos(Input::GetMousePositionInViewport(), 1.0f);

            glm::vec3 dir  = (v1 - v0); 
            dir = glm::normalize(dir);

            //Project from screen plane into worldspace based on initial distance from vertex to camera
            glm::vec3 mouseposprojected = v0 + (dir * m_mouseHit.Distance);
            m_mouseDownVert->bFixed = true;
            m_mouseDownVert->Position = mouseposprojected;
            m_mouseDownVert->OldPosition = mouseposprojected;
            
            
        }
    }
    UpdateMesh();
}

void ClothActor::Draw()
{
    Actor::Draw();

    //Draw cloth mesh
    m_mesh.Draw(m_transform);

    //Draw collision mesh
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_collisionMeshCurrentPosition);
    model = glm::scale(model, glm::vec3(m_collisionMeshScale, m_collisionMeshScale, m_collisionMeshScale));
    if (collisionEnabled) m_collisionMesh.Draw(m_transform * model);

    //Draw a hook on any fixed vertices
    for (auto p : m_particles)
    {
        if (p.bFixed)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), p.Position);
            //model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
            m_hook.Draw(m_transform * model);
        }
    }

    
}

void ClothActor::Begin()
{
    //Initialize the mesh
    Actor::Begin();
    TraceEnabled = true;
    GenerateCloth(width, height);
    m_collisionMesh.Load("Resources/Meshes/Sphere.obj");
    m_collisionMesh.SetTexture("Resources/Textures/Wood.jpg");
    m_hook.Load("Resources/Meshes/Hook.obj");
    m_hook.SetTexture("Resources/Textures/Wood.jpg");
    
}

void ClothActor::OnDestroy()
{
    Actor::OnDestroy();
}

nlohmann::json ClothActor::Serialize()
{
    nlohmann::json j = Actor::Serialize();
    j["Width"] = width;
    j["Height"] = height;
    j["Iterations"] = iterations;
    j["PhysicsTime"] = physicsTime;

    j["RestLength"] = restLength;
    j["Stiffness"] = stiffness;
    j["Damping"] = damping;
    j["Gravity"] = m_gravity;
    j["Wind"] = m_wind;
    j["Turbulence"] = windTurbulenceStrength;
    j["WindFrequency"] = windFrequency;
    j["SelfCollision"] = selfCollisionEnabled;
    
    j["CollisionEnabled"] = collisionEnabled;
    j["CollisionMeshOrigin"] = m_collisionMeshOrigin;
    j["CollisionMeshScale"] = m_collisionMeshScale;
    j["CollisionMeshSpeed"] = m_collisionMeshSpeed;
    j["CollisionMeshTravelDistance"] = m_collisionTravelDistance;
    

    return j;
}

void ClothActor::Deserialize(nlohmann::json& j)
{
    Actor::Deserialize(j);
    width = j["Width"];
    height = j["Height"];
    iterations = j["Iterations"];
    physicsTime = j["PhysicsTime"];

    restLength = j["RestLength"];
    stiffness = j["Stiffness"];
    damping = j["Damping"];
    m_gravity = j["Gravity"];
    m_wind = j["Wind"];
    if (j.contains("Turbulence")) windTurbulenceStrength = j["Turbulence"];
    if (j.contains("WindFrequency")) windFrequency = j["WindFrequency"];
    if (j.contains("SelfCollision")) selfCollisionEnabled = j["SelfCollision"];

    collisionEnabled = j["CollisionEnabled"];
    m_collisionMeshOrigin = j["CollisionMeshOrigin"];
    m_collisionMeshScale = j["CollisionMeshScale"];
    m_collisionMeshSpeed = j["CollisionMeshSpeed"];
    m_collisionTravelDistance = j["CollisionMeshTravelDistance"];
    

    GenerateCloth(width, height);
}

void ClothActor::OnInspectorGUI()
{
    Actor::OnInspectorGUI();

    if (ImGui::Button("Reset Cloth"))
    {
        GenerateCloth(width, height);
    }
    
    if(ImGui::CollapsingHeader("Cloth Attributes"))
    {
        if (ImGui::DragFloat("Curtain Rail Distance", &curtainRailDistance, 0.01f, 0.0f, 1.f))
        {
            
            for (int i = 0; i < width; i++)
            {
                int index = i + (height - 1) * width;
                if (m_particles[index].bFixed) m_particles[index].Position.x = curtainRailDistance * i;
            }
        }
        
        
        if (ImGui::DragInt("Width", &width, 1, 1, 100))
        {
            GenerateCloth(width, height);   
        }
        if (ImGui::DragInt("Height", &height, 1, 1, 100))
        {
            GenerateCloth(width, height);
        }
        if (ImGui::DragFloat("Rest Distance", &restLength, 0.1f, 0.1f, 10.0f))
        {

        }
    }
    
    if (ImGui::CollapsingHeader("Solver"))
    {
        if (ImGui::DragInt("Iterations", & iterations, 1, 1, 16))
        {}
        
        if (ImGui::DragFloat("PhysicsTime", &physicsTime, 1.f, 0.0f, 10000.f))
        {}

        if (ImGui::DragFloat3("Wind", &m_wind.x, 0.1f, -10.0f, 10.0f))
        {}

        if (ImGui::DragFloat("Turbulence", &windTurbulenceStrength, 0.1f, 0.0f, 50.0f))
        {}

        if (ImGui::DragFloat("Wind Frequency", &windFrequency, 0.1f, 0.0f, 50.0f))
        {}

        if (ImGui::DragFloat3("Gravity", &m_gravity.x, 0.1f, -10.0f, 10.0f))
        {}

        if (ImGui::DragFloat("Stiffness", &stiffness, 0.01f, 0.0f, 1.0f))
        {}

        if (ImGui::DragFloat("Damping", &damping, 0.01f, 0.0f, 1.0f))
        {}
    }

    if (ImGui::CollapsingHeader("Collider"))
    {
        if (ImGui::Checkbox("Self Collision", &selfCollisionEnabled))
        {}
        
        if (ImGui::Checkbox("Collision##Collider", &collisionEnabled))
        {}
        
        if (ImGui::DragFloat3("Position##Collider", &m_collisionMeshOrigin.x, 0.1f, -100.0f, 100.0f))
        {}

        if (ImGui::DragFloat3("Travel Distance##Collider", &m_collisionTravelDistance.x, 0.1f, -100.0f, 100.0f))
        {}

        if (ImGui::DragFloat("Scale##Collider", &m_collisionMeshScale, 0.1f, 0.0f, 100.0f))
        {}

        if (ImGui::DragFloat("Speed##Collider", &m_collisionMeshSpeed, 0.1f, 0.0f, 100.0f))
        {}

        
        
    }
    
}

void ClothActor::ResolveCollisions(float ts)
{

    //Loop through all cloth mesh particles
    for (int i = 0; i < m_particles.size(); i++)
    {
        //Get particle position in world space
        glm::vec3 particlePos = m_particles[i].Position + GetPosition();
        
        //Check if particle is within collision mesh sphere
        if (glm::distance(particlePos, m_collisionMeshCurrentPosition) < (m_collisionMeshScale * 1.1f) )
        {
            //Get particle position in collision mesh space
            glm::vec3 particlePosInCollisionMeshSpace = particlePos - m_collisionMeshCurrentPosition;
            
            //Get closest point on collision mesh
            const Vertex& closestPoint = m_collisionMesh.GetClosestPoint(particlePosInCollisionMeshSpace);
            
            //Get normal of closest point
            glm::vec3 normal = closestPoint.Normal;
            
            //Get distance between particle and closest point
            float distance = glm::distance(particlePosInCollisionMeshSpace, closestPoint.Position) ;
            
            //Get penetration depth
            float penetrationDepth = m_collisionMeshScale - distance;
            
            //Get penetration vector
            glm::vec3 penetrationVector = penetrationDepth * normal * -0.05f;
            
            //Move particle out of collision mesh
            m_particles[i].Position -= penetrationVector;
        }



        
        // Check if particle collides with any other actor
        for (auto& actor : m_scene->GetActorsOfClass<DynamicMeshActor>())
        {
            if (actor == this) continue;

            //Get particle position in world space
            glm::vec3 particlePos = m_particles[i].Position + GetPosition();

            //Assume mesh is a plane. Check for collision with plane
            if (actor->GetMesh().GetVertices().size() == 4)
            {
                //Get plane normal
                glm::vec3 planeNormal = actor->GetMesh().GetVertices()[0].Normal;

                //Get plane position
                glm::vec3 planePos = actor->GetPosition();

                //Plane AABB intersection
                if (particlePos.x > planePos.x - actor->GetScale().x && particlePos.x < planePos.x + actor->GetScale().x &&
                    particlePos.y > planePos.y - 1.f && particlePos.y < planePos.y + 0.2f &&
                    particlePos.z > planePos.z - actor->GetScale().z && particlePos.z < planePos.z + actor->GetScale().z)
                {
                    //Get distance between particle and plane
                    float distance = glm::dot(planeNormal, particlePos - planePos);

                    //Get penetration depth
                    float penetrationDepth = 1.0f - distance;

                    //Get penetration vector
                    glm::vec3 penetrationVector = penetrationDepth * planeNormal * -0.05f;

                    //Move particle out of collision mesh
                    if (!m_particles[i].bFixed) m_particles[i].Position -= penetrationVector; m_particles[i].OldPosition = m_particles[i].Position;
                }
            }
        }

        //Expensive. Only run if explicitly enabled
        if (selfCollisionEnabled)
        {
            if (m_particles[i].bCollisionParticle && !m_particles[i].bFixed)
            {
                for (int j = 0; j < m_particles.size(); j++)
                {
                    //Dont run if this is the same particle or if the other particle is not a collision particle
                    if (i == j) continue;
                    if (!m_particles[j].bCollisionParticle) continue;

                    // Check if they are too close
                    if (glm::distance(m_particles[i].Position, m_particles[j].Position) < 0.5f)
                    {
                        // Offset particle to avoid collision
                        m_particles[i].Position += glm::normalize(m_particles[i].Position - m_particles[j].Position) * 0.1f; m_particles[i].OldPosition = m_particles[i].Position;
                    
                    }
                }
            }
        }

        
        
    }
    
}


void ClothActor::GenerateCloth(int width, int height)
{
    m_particles.clear();
    m_constraints.clear();
    m_mesh.Clear();

    this->width = width;
    this->height = height;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    
    //Create particles
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = y * width + x;
            ClothParticle& p = m_particles.emplace_back();
            p.Position = glm::vec3(x, y, 0);
            p.OldPosition = p.Position;
            p.Velocity = glm::vec3(0, 0, 0);

            //Top left, top right and top middle particles are fixed
            p.bFixed = (y == height - 1 && (x == 0 || x == width - 1 || x == width / 2));

            //All edge particles and every second particle is used to test collision
            if (y % 2 == 0 || x % 2 == 0 || y == height - 1 || y == 0 || x == 0 || x == width - 1)
            {
                p.bCollisionParticle = true;
            }
        }
    }

    //Create constraints
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = y  * width + x;
            if (x < width - 1)
            {
                m_constraints.emplace_back(&m_particles[index], &m_particles[index + 1], restLength);
            }
            if (y < height - 1)
            {
                m_constraints.emplace_back(&m_particles[index], &m_particles[index + width], restLength);
            }
            if (x < width - 1 && y < height - 1)
            {
                m_constraints.emplace_back(&m_particles[index], &m_particles[index + width + 1], restLength * sqrt(2));
                m_constraints.emplace_back(&m_particles[index + 1], &m_particles[index + width], restLength * sqrt(2));
            }
        }
    }

    //Derive mesh from particles, then initialise the cloth mesh
    GetVertices(vertices, indices);
    m_mesh.Create(vertices, indices);
}

void ClothActor::VerletIntegration(float deltaTime)
{
    for (auto& p : m_particles)
    {
        if (!p.bFixed)
        {
            glm::vec3 FinalForce = { 0,0,0 };

            //convert gravity from world space to local space
            glm::vec3 gravity = glm::vec3(glm::inverse(m_transform) * glm::vec4(m_gravity, 0));

            //convert wind from world space to local space
            glm::vec3 wind = glm::vec3(glm::inverse(m_transform) * glm::vec4(m_wind, 0));

            // apply some turbulent wind using perlin noise
            wind.x += (perlin.normalizedOctave1D(glfwGetTime() * windFrequency + p.Position.x, 3, 0.5)) * windTurbulenceStrength;
            wind.y += (perlin.normalizedOctave1D(glfwGetTime() * windFrequency + p.Position.y, 3, 0.5)) * windTurbulenceStrength;
            wind.z += (perlin.normalizedOctave1D(glfwGetTime() * windFrequency + p.Position.z, 3, 0.5)) * windTurbulenceStrength;
            
            
            //Apply Gravity
            FinalForce += gravity;
            //Apply Wind
            FinalForce += wind;
            //Apply Drag
            FinalForce += -damping * p.Velocity;
            
            //Verlet Integration
            glm::vec3 temp = p.Position;
            p.Position += (p.Position - p.OldPosition) + (FinalForce * deltaTime * deltaTime);
            p.OldPosition = temp;
            p.Velocity = (p.Position - p.OldPosition) / deltaTime;
            
        }
    }
    
}

void ClothActor::SatisfyConstraints(int iterations, float ts)
{
    //Can run multiple iterations to make the cloth more rigid & stable
    for (int i = 0; i < iterations; i++)
    {
        for (auto& c : m_constraints)
        {
            ClothParticle* p1 = c.P1;
            ClothParticle* p2 = c.P2;

            //Get the current distance between the two particles
            glm::vec3 delta = p1->Position - p2->Position;
            float distance = glm::length(delta);

            
            if (distance > 0)
            {
                //Correct particle positions
                float difference = (c.RestLength - distance) / distance ;
                glm::vec3 translate = delta * difference * 0.5f * stiffness * ts;
                
                if (!p1->bFixed)
                {
                    p1->Position += translate;
                }
                if (!p2->bFixed)
                {
                    p2->Position -= translate;
                }
            }
        }
    }


    
    
}

void ClothActor::GetVertices(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
    
    if (m_particles.size() == 0)
        return;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            //Front Faces
            ClothParticle& p = m_particles[y * width + x];

            // Create vertex
            Vertex& v = vertices.emplace_back();
            v.Position = p.Position;
            v.TexCoord = glm::vec2(x / (float)width, y / (float)height);
            v.Colour = glm::vec3(1.f, 1.f, 1.f);
            v.Normal = {0, 0, 1};

            // Create indices
            if (x < width - 1 && y < height - 1)
            {
                indices.push_back(y * width + x);
                indices.push_back(y * width + x + 1);
                indices.push_back((y + 1) * width + x);

                indices.push_back(y * width + x + 1);
                indices.push_back((y + 1) * width + x + 1);
                indices.push_back((y + 1) * width + x);
            }
            
        }
    }

    //Generate Back faces for shading. These get slapped on to the back because I thought about this too late and did not want to rework it
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            //Back faces
            ClothParticle& p = m_particles[y * width + x];

            // Create vertex
            Vertex& v = vertices.emplace_back();
            v.Position = p.Position;
            v.TexCoord = glm::vec2(x / (float)width, y / (float)height);
            v.Colour = glm::vec3(1.f, 1.f, 1.f);
            v.Normal = { 0, 0, -1 };

            // Create indices
            if (x < width - 1 && y < height - 1)
            {
                indices.push_back(width * height + y * width + x);
                indices.push_back(width * height + (y + 1) * width + x);
                indices.push_back(width * height + y * width + x + 1);

                indices.push_back(width * height + y * width + x + 1);
                indices.push_back(width * height + (y + 1) * width + x);
                indices.push_back(width * height + (y + 1) * width + x + 1);
            }
        }
    }
    
    

    // Calculate triangle normals using cross product
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        uint32_t i0 = indices[i];
        uint32_t i1 = indices[i + 1];
        uint32_t i2 = indices[i + 2];

        glm::vec3 v1 = vertices[i1].Position - vertices[i0].Position;
        glm::vec3 v2 = vertices[i2].Position - vertices[i0].Position;

        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        vertices[i0].Normal += normal;
        vertices[i1].Normal += normal;
        vertices[i2].Normal += normal;
    }
    
}

void ClothActor::UpdateMesh()
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    GetVertices(vertices, indices);
    m_mesh.Update(vertices, indices);
}

ClothActor::~ClothActor()
{
}
