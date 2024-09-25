#include "UIComponent.h"
#include <iostream>


UIComponent::UIComponent()
{
}


void UIComponent::Init()
{
}

void UIComponent::Begin()
{
	std::string _id = std::to_string(id);
	std::string windowname = name + "##" + _id;

	bool* canclose = canBeClosed ? &isOpen : nullptr;

	ImGui::Begin(windowname.c_str(), canclose);
}

void UIComponent::Update(Timestep ts)
{

}

void UIComponent::End()
{
	ImGui::End();
}
