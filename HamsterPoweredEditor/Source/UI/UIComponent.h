#pragma once
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Core/Timestep.h"
#include <string>
class ImGuiLayer;

class UIComponent
{
public:
	virtual ~UIComponent() = default;
	UIComponent();
	UIComponent(const UIComponent& other) = delete;

	virtual void Init();

	virtual void Begin();
	virtual void Update(Timestep ts);
	virtual void End();

	bool canBeClosed = true;

protected:
	std::string name = "Widget";
	ImGuiLayer* m_Parent = nullptr;

private:
	bool isOpen = true;
	int id = 0;
	friend class ImGuiLayer;
	
};

