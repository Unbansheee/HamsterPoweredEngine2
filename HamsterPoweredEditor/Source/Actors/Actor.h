#pragma once
#include <functional>
#include <string>
#include <vector>
#include "glm/glm.hpp"

#include "Object.h"
#include "Rendering/Renderer.h"
#include "json.hpp"
#include "Core/UUID.h"

class Scene;



namespace nlohmann
{
    template <>
    struct adl_serializer<glm::vec3>
    {
        static void to_json(json& j, const glm::vec3& vec)
        {
            j = json{ vec.x, vec.y, vec.z };
        }

        static void from_json(const json& j, glm::vec3& vec)
        {
            vec.x = j[0];
            vec.y = j[1];
            vec.z = j[2];
        }
    };

    template <>
    struct adl_serializer<glm::vec4>
    {
        static void to_json(json& j, const glm::vec4& vec)
        {
            j = json{ vec.x, vec.y, vec.z, vec.w };
        }

        static void from_json(const json& j, glm::vec4& vec)
        {
            vec.x = j[0];
            vec.y = j[1];
            vec.z = j[2];
            vec.w = j[3];
        }
    };

    template <>
    struct adl_serializer<glm::vec2>
    {
        static void to_json(json& j, const glm::vec2& vec)
        {
            j = json{ vec.x, vec.y };
        }

        static void from_json(const json& j, glm::vec2& vec)
        {
            vec.x = j[0];
            vec.y = j[1];
        }
    };
    
}

class Actor : public Object
{
public:
    Actor();
    Actor(std::string name);
    Actor(std::string name, glm::vec3 position = {0, 0, 0}, glm::vec3 rotation = {0, 0, 0}, glm::vec3 scale = {1, 1, 1});

    void Destroy();

    void Update(Timestep ts) override;
    
    void SetPosition(float x, float y, float z = 0.0f);
    void SetPosition(glm::vec3 position);
    void AddPositionOffset(float x, float y, float z = 0.0f);
    void AddPositionOffset(glm::vec3 position);
    void SetRelativePosition(float x, float y, float z = 0.0f);
    void SetRelativePosition(glm::vec3 position);
    glm::vec3 GetPosition();
    glm::vec3 GetRelativePosition();
    
    void SetRotation(float x, float y, float z = 0.0f);
    void SetRotation(glm::vec3 rotation);
    void AddRotationOffset(float x, float y, float z = 0.0f);
    void AddRotationOffset(glm::vec3 rotation);
    void SetRelativeRotation(float x, float y, float z = 0.0f);
    void SetRelativeRotation(glm::vec3 rotation);
    glm::vec3 GetRotation();
    glm::vec3 GetRelativeRotation();

    void SetTransparency(bool state) { m_Transparency = state; }
    
    void SetScale(float x, float y, float z = 1.0f);
    void SetScale(glm::vec3 scale);
    void SetScale(float scale);
    void AddScaleOffset(float x, float y, float z = 0.0f);
    void AddScaleOffset(glm::vec3 scale);
    void SetRelativeScale(float x, float y, float z = 0.0f);
    void SetRelativeScale(glm::vec3 scale);
    glm::vec3 GetScale();
    glm::vec3 GetRelativeScale();

    void AddChild(Actor* child);
    void RemoveChild(Actor* child);
    void RemoveAllChildren();
    std::vector<Actor*> GetChildren();

    void SetParent(Actor* parent, bool attachPosition = true, bool attachRotation = true, bool attachScale = true);
    Actor* GetParent();
    void SetParentAttachment(bool position, bool rotation, bool scale);
    void DetachFromParent();

    const glm::mat4& GetTransform();

    template<typename T, typename... Args>
    T* CreateChild(Args&&... args)
    {
        T* child = new T(std::forward<Args>(args)...);
        child->SetParent(this);
        m_children.push_back(child);
        return child;
    }

    void SetUpdateCallback(std::function<void()> callback) { m_updateCallback = callback; }

    virtual void Draw();

    void SetRenderSettings(RenderSettings settings) { m_renderSettings = settings; }
    void SetBlending(bool state) { m_renderSettings.Blending = state; }
    void SetDepthTest(bool state) { m_renderSettings.DepthTest = state; }
    void SetDepthWrite(bool state) { m_renderSettings.DepthWrite = state; }
    void SetCullFace(bool state) { m_renderSettings.Culling = state; }

    virtual void Begin() override {}
    virtual void OnDestroy() override {}
    
    template <typename T>
    bool Is()
    {
        return Cast<T>();
    }
    
    template <typename T>
    T* Cast()
    {
        return dynamic_cast<T*>(this);
    }

    nlohmann::json Serialize() override;
    void Deserialize(nlohmann::json& j) override;

    const HP::UUID& GetID() { return m_id; }
    
protected:
    RenderSettings m_renderSettings;
    friend class CameraController;
    friend class Scene;
    void UpdateTransform();
    
    std::vector<Actor*> m_children;
    glm::mat4 m_transform;

    Scene* m_scene;
    Actor* m_parent;
    bool m_attachedToParent;
    glm::vec3 m_parentOffset;

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale = glm::vec3(1.0f);

    bool m_Transparency = false;

    std::function<void()> m_updateCallback;

    HP::UUID m_id;

    void SetID(const std::string& id) { m_id.SetID(id); }
    
protected:
    friend class InspectorPanel;
    virtual void OnInspectorGUI()
    {
        return;
    }
};
