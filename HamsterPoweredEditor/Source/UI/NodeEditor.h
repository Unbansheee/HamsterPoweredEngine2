#pragma once
#include <vector>

#include "UIComponent.h"

class NodeEditor : public UIComponent
{
public:
    ~NodeEditor() override;
    void Init() override;
    void Begin() override;
    void Update(Timestep ts) override;
    void End() override;

private:
    std::vector<std::pair<int, int>> links;
};
