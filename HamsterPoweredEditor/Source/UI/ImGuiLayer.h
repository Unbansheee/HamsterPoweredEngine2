#pragma once
#include <vector>
#include <list>
#include <memory>
#include <string>

#include "imgui.h"
#include "Core/Window.h"

class Timestep;
class Actor;

enum resizeDirection {
	RESIZE_NONE,
	RESIZE_LEFT,
	RESIZE_RIGHT,
	RESIZE_TOP,
	RESIZE_BOTTOM,
	RESIZE_TOPLEFT,
	RESIZE_TOPRIGHT,
	RESIZE_BOTTOMLEFT,
	RESIZE_BOTTOMRIGHT
};

class UIComponent;
class ImGuiLayer
{
public:
	ImGuiLayer(Window* window);
	~ImGuiLayer();
	
	void Begin();
	void BeginFrame();
	void EndFrame();
	void Update(Timestep ts);

	void SaveAs();
	void Save();
	void Load();

	template <typename T>
	T* CreateWidget();

	std::vector<UIComponent*> UIComponents;
	Window* window;
	int count = 0;
	bool windowDragged = false;
	bool resizeInProgress = false;
	bool wantsToResize = false;
	int mouseRel[2];
	resizeDirection resizeDir = RESIZE_NONE;
	Actor* m_SelectedActor = nullptr;
};

template<typename T>
inline T* ImGuiLayer::CreateWidget()
{
	T* widget(new T());
	widget->m_Parent = this;
	widget->Init();
	widget->id = count;
	UIComponents.push_back(widget);
	count++;
	return widget;
}
