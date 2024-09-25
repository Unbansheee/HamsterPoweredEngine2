#pragma once
#include "UIComponent.h"
class Stats :
    public UIComponent
{
public:
    void Init() override;
    void Update(Timestep ts) override;
};

