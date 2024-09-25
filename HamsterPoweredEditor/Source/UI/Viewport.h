#pragma once
#include "UIComponent.h"
class Viewport : public UIComponent
{
public:
	void Init() override;
	void Update(Timestep ts) override;
	void Begin() override;
	void End() override;

private:
	friend class App;
	ImVec2 lastSize;
	ImVec2 lastPos;
	bool mouseCaptured = false;
};

