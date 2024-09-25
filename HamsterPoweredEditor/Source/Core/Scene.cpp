#include "Scene.h"

#include <fstream>
#include <iostream>

#include "Timer.h"
#include "Actors/AnimatedQuad.h"
#include "Actors/Hexagon.h"

#include "Actors/Actor.h"
#include "Actors/Quad.h"
#include "Actors/TextLabel.h"
#include "Actors/MeshActor.h"
#include "json.hpp"
#include "Actors/ClothActor.h"
#include "Actors/DirectionalLight.h"
#include "Actors/DynamicMeshActor.h"
#include "Actors/PointLight.h"
#include "Actors/SkyboxActor.h"
#include "Actors/Spinner.h"

Scene::Scene()
{
    //Temporary editor camera. This should be moved to Scene or UI Layer later
    m_editorCamera = new CameraController(CameraController::CameraType::ORTHO, (float)Renderer::GetViewportSize().x / (float)Renderer::GetViewportSize().x);
    m_editorCamera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    m_editorCamera->SetName("Editor Camera");
    m_editorCamera->SetZoom(2.5f);



    Renderer::SetClearColor(m_sceneColour);
}

Scene::~Scene()
{
    for (auto& actor : m_actors)
    {
        delete actor;
    }

    delete m_editorCamera;
    
}

void Scene::Update(Timestep ts)
{
    m_fixedUpdateAccumulator += ts.GetSeconds();
    while (m_fixedUpdateAccumulator >= m_fixedUpdateInterval)
    {
        m_fixedUpdateAccumulator -= m_fixedUpdateInterval;
        for (auto& actor : m_actors)
        {
            actor->FixedUpdate(m_fixedUpdateInterval);
        }
    }
    
    m_editorCamera->Update(ts);
    for (Actor* actor : m_actors)
    {
        actor->Update(ts);
    }
}

void Scene::Render()
{
    for (Actor* actor : m_actors)
    {
        actor->Draw();
    }

}

void Scene::Begin()
{
    
    for (Actor* actor : m_actors)
    {
        actor->Begin();
    }
    
}

void Scene::DestroyActor(Actor* actor)
{
    m_actorsToDestroy.push_back(actor);
}

void Scene::SetColour(glm::vec4 colour)
{
    m_sceneColour = colour;
    Renderer::SetClearColor(colour);
}

void Scene::SerializeScene(const std::string& filepath)
{
    
    nlohmann::json j;
    j["EditorCamera"] = m_editorCamera->Serialize();

    j["SceneColour"] = m_sceneColour;
    j["AmbientIntensity"] = Renderer::m_AmbientLightStrength;
    j["RenderMode"] = Renderer::GetRenderMode();
    
    j["Actors"] = nlohmann::json::array();
    for (Actor* actor : m_actors)
    {
        nlohmann::json actorJson = actor->Serialize();
        j["Actors"].push_back(actorJson);
    }

    std::ofstream file(filepath);
    file << j.dump(4);
    
    
}

void Scene::DeserializeScene(const std::string& filepath)
{
    nlohmann::json j;
    std::ifstream file(filepath);
    file >> j;

    if (j.contains("EditorCamera"))
    {
        m_editorCamera->Deserialize(j["EditorCamera"]);
    }

    if (j.contains("SceneColour"))
    {
        m_sceneColour = j["SceneColour"];
        Renderer::SetClearColor(m_sceneColour);
    }

    if (j.contains("RenderMode"))
    {
        Renderer::SetRenderMode(j["RenderMode"]);
    }

    if (j.contains("AmbientIntensity"))
    {
        Renderer::m_AmbientLightStrength = j["AmbientIntensity"];
    }
    

     for (auto& actorJson : j["Actors"])
    {
        std::string actorType = actorJson["ActorType"];
        Actor* actor = nullptr;
        if (actorType == "Actor")
        {
            actor = SpawnActor<Actor>();
        }
        else if (actorType == "Quad")
        {
            actor = SpawnActor<Quad>();
        }
        else if (actorType == "AnimatedQuad")
        {
            actor = SpawnActor<AnimatedQuad>();
        }
        else if (actorType == "Hexagon")
        {
            actor = SpawnActor<Hexagon>();
        }
        else if (actorType == "Mesh" || actorType == "MeshActor")
        {
            actor = SpawnActor<MeshActor>();
        }
        else if (actorType == "TextLabel")
        {
            actor = SpawnActor<TextLabel>();
        }
         else if (actorType == "DirLight")
         {
             actor = SpawnActor<DirLight>();
         }
         else if (actorType == "PointLight")
         {
             actor = SpawnActor<PointLight>();
         }
         else if (actorType == "Spinner")
         {
             actor = SpawnActor<Spinner>();
         }
         else if (actorType == "ClothActor")
         {
             actor = SpawnActor<ClothActor>();
         }
         else if (actorType == "DynamicMeshActor")
         {
             actor = SpawnActor<DynamicMeshActor>();
         }
         else if (actorType == "SkyboxActor")
         {
             actor = SpawnActor<SkyboxActor>();
         }

        else
        {
            std::cout << "Unknown actor type: {0}" << actorType << std::endl;
            continue;
        }
        actor->Deserialize(actorJson);
    }

    
    while (m_parentChildQueue.size() > 0)
    {
        Actor* parent = GetActorByID(m_parentChildQueue.front().first);
        Actor* child = GetActorByID(m_parentChildQueue.front().second);
        if (parent && child)
        {
            parent->AddChild(child);
        }
        m_parentChildQueue.pop();
    }
    
    
}

Actor* Scene::GetActorByID(const HP::UUID& id)
{
    for (Actor* actor : m_actors)
    {
        if (actor->GetID() == id)
        {
            return actor;
        }
    }
    return nullptr;
}


void Scene::SetParentChild(const HP::UUID& parentID, const HP::UUID& childID)
{
    m_parentChildQueue.push({ parentID, childID });
}


void Scene::DeferredDestroy()
{
    for (int i = 0; i < m_actorsToDestroy.size(); i++)
    {
        auto it = std::find(m_actors.begin(), m_actors.end(), m_actorsToDestroy[i]);
        m_actorsToDestroy[i]->DetachFromParent();
        delete *it;
        m_actors.erase(it);
        
    }
    m_actorsToDestroy.clear();
}
