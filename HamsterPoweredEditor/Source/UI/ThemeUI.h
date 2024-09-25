#pragma once
#include "UIComponent.h"

class ThemeUI :
    public UIComponent
{
public:
    void Update(Timestep ts);
    void Init();
};

