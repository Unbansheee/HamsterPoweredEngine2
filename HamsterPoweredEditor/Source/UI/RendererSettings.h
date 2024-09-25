#pragma once
#include "UIComponent.h"

class RendererSettings : public UIComponent
{
public:
    void Init() override;
    void Begin() override;
    void Update(Timestep ts) override;
    void End() override;
};
