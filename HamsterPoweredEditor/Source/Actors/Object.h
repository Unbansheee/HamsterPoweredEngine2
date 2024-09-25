#pragma once
#include <string>

#include "Core/Timestep.h"
#include "json.hpp"

class Object
{
public:
    virtual ~Object() = default;
    virtual void Begin() = 0;
    virtual void Update(Timestep ts) {};
    virtual void FixedUpdate(double ts) {};
    virtual void OnDestroy() = 0;
    
    virtual nlohmann::json Serialize() = 0;
    virtual void Deserialize(nlohmann::json& j) = 0;

    void SetName(std::string name);
    std::string GetName();

protected:
    std::string m_name = "Object";
};
