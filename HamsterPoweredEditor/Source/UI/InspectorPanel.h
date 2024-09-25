#pragma once
#include "UIComponent.h"
#include "Actors/Actor.h"

class InspectorPanel : public UIComponent
{
public:
    void DrawTransformEdit(Actor* actor);
    void Init() override;
    void Begin() override;
    void Update(Timestep ts) override;
    void End() override;


private:
    bool m_lockedScale = true;
};

