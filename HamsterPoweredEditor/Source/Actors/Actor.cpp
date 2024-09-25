#include "Actors/Actor.h"
#include "Core/Scene.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

Actor::Actor()
{
    
}

Actor::Actor(std::string name)
{
    SetName(name);
}

Actor::Actor(std::string name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
    m_position(position),
    m_rotation(rotation),
    m_scale(scale)
{
    SetName(name);
    UpdateTransform();
}


void Actor::Destroy()
{
    OnDestroy();
    m_scene->DestroyActor(this);
    for (auto child : m_children)
    {
        child->Destroy();
    }
}

void Actor::Update(Timestep ts)
{
    //Object::Update(ts);
    if (m_updateCallback)
    {
        m_updateCallback();
    }
    
}

void Actor::SetPosition(float x, float y, float z)
{
    m_position = glm::vec3(x, y, z);
    UpdateTransform();
}

void Actor::SetPosition(glm::vec3 position)
{
    m_position = position;
    UpdateTransform();
}

void Actor::AddPositionOffset(float x, float y, float z)
{
    m_position += glm::vec3(x, y, z);
    UpdateTransform();
}

void Actor::AddPositionOffset(glm::vec3 position)
{
    m_position += position;
    UpdateTransform();
}

void Actor::SetRelativePosition(float x, float y, float z)
{
}

void Actor::SetRelativePosition(glm::vec3 position)
{
}

glm::vec3 Actor::GetPosition()
{
    return m_position;
}

glm::vec3 Actor::GetRelativePosition()
{
    //return glm::vec3(m_transform[3][0], m_transform[3][1], m_transform[3][2]);
    return glm::vec3(0, 0, 0);
}

void Actor::SetRotation(float x, float y, float z)
{
    m_rotation = glm::vec3(x, y, z);
    UpdateTransform();
}

void Actor::SetRotation(glm::vec3 rotation)
{
    m_rotation = rotation;
    UpdateTransform();
}


void Actor::AddRotationOffset(float x, float y, float z)
{
    m_rotation += glm::vec3(x, y, z);
    UpdateTransform();    
}

void Actor::AddRotationOffset(glm::vec3 rotation)
{
    m_rotation += rotation;
    UpdateTransform();
}

void Actor::SetRelativeRotation(float x, float y, float z)
{
}

void Actor::SetRelativeRotation(glm::vec3 rotation)
{
}

glm::vec3 Actor::GetRotation()
{
    return m_rotation;
}

glm::vec3 Actor::GetRelativeRotation()
{
    return glm::vec3(0, 0, 0);
}

void Actor::SetScale(float x, float y, float z)
{
    m_scale = glm::vec3(x, y, z);
    UpdateTransform();
}

void Actor::SetScale(glm::vec3 scale)
{
    m_scale = scale;
    UpdateTransform();
}

void Actor::SetScale(float scale)
{
    m_scale = glm::vec3(scale, scale, scale);
    UpdateTransform();
}

void Actor::AddScaleOffset(float x, float y, float z)
{
    m_scale += glm::vec3(x, y, z);
    UpdateTransform();
}

void Actor::AddScaleOffset(glm::vec3 scale)
{
    m_scale += scale;
    UpdateTransform();
}

void Actor::SetRelativeScale(float x, float y, float z)
{
}

void Actor::SetRelativeScale(glm::vec3 scale)
{
}

glm::vec3 Actor::GetScale()
{
    return m_scale;
}

glm::vec3 Actor::GetRelativeScale()
{
    return glm::vec3(0, 0, 0);
}

void Actor::AddChild(Actor* child)
{
    if (child)
    {
        m_children.push_back(child);
        child->m_parent = this;
        child->UpdateTransform();
    }
}

void Actor::RemoveChild(Actor* child)
{
    if (child)
    {
        auto it = std::find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end())
        {
            m_children.erase(it);
            child->m_parent = nullptr;
            child->UpdateTransform();
        }
    }
}

void Actor::RemoveAllChildren()
{
    for (auto child : m_children)
    {
        RemoveChild(child);
    }
}

std::vector<Actor*> Actor::GetChildren()
{
    return m_children;
}

void Actor::SetParent(Actor* parent, bool attachPosition, bool attachRotation, bool attachScale)
{
    DetachFromParent();
    
    if (parent)
    {
        m_parent = parent;
        parent->m_children.push_back(this);
        UpdateTransform();
    }
}

Actor* Actor::GetParent()
{
    return m_parent;
}

void Actor::SetParentAttachment(bool position, bool rotation, bool scale)
{
}

void Actor::DetachFromParent()
{
    if (m_parent)
    {
        m_parent->RemoveChild(this);
        m_parent = nullptr;
    }
    UpdateTransform();
}

const glm::mat4& Actor::GetTransform()
{
    return m_transform;
}

void Actor::Draw()
{
}

nlohmann::json Actor::Serialize()
{
    nlohmann::json json;

    std::string type = typeid(*this).name();

    if (type.find("class ") == 0)
    {
        type = type.substr(6);
    }
    else if (type.find("struct ") == 0)
    {
        type = type.substr(7);
    }

    json["ID"] = m_id.GetAsString();
    
    json["ActorType"] = type;
    json["name"] = GetName();

    nlohmann::json children;
    for (auto child : m_children)
    {
        children.push_back(child->GetID().GetAsString());
    }
    json["children"] = children;
    
    json["position"] = GetPosition();
    json["rotation"] = GetRotation();
    json["scale"] = GetScale();
    

    return json;
    
}

void Actor::Deserialize(nlohmann::json& j)
{
    SetName(j["name"] );

    if (j.contains("ID"))
    {
        if (!j["ID"].empty()) m_id.SetID(j["ID"]);
    }
    
    if (j.contains("position"))
    {
        SetPosition(j["position"]);
    }
    if (j.contains("rotation"))
    {
        SetRotation(j["rotation"]);
    }
    if (j.contains("scale"))
    {
        SetScale(glm::vec3(j["scale"]));
    }
    if (j.contains("children"))
    {
        for (std::string child : j["children"])
        {
            m_scene->SetParentChild(GetID(), HP::UUID(child));
        }
    }
    UpdateTransform();


    
}

void Actor::UpdateTransform()
{

    glm::mat4 parentTransform = glm::mat4(1.0f);
    if (m_parent)
    {
        parentTransform = m_parent->GetTransform();
    }
    
    
    glm::mat4 s = glm::scale(glm::mat4(1.0f), m_scale);
    glm::mat4 r = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
    r = glm::rotate(r, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
    r = glm::rotate(r, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
    glm::mat4 t = glm::translate(parentTransform, m_position);
    
    m_transform = t * r * s;
    

    for (auto child : m_children)
    {
        child->UpdateTransform();
    }
}
