#pragma once
#include "Actors/Actor.h"
#include "UIComponent.h"

class HierarchyPanel : public UIComponent
{
public:
    void Init() override;
    void Begin() override;
    void RenderTree(Actor* actor, int& index);
    void Update(Timestep ts) override;
    void End() override;

private:
    
};
