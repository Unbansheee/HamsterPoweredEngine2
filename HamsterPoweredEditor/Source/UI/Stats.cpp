#include "Stats.h"

#include <iostream>

#include "Core/Input.h"
#include "Rendering/Renderer.h"

void Stats::Init()
{
	name = "Statistics";
	canBeClosed = true;
}

void Stats::Update(Timestep ts)
{
	ImGui::Text("Mouse Position: X: %i, Y: %i", (int)Input::GetMousePosition().x, (int)Input::GetMousePosition().y);
	ImGui::Text("Absolute Mouse Position: X: %i, Y: %i", (int)Input::GetMousePositionAbsolute().x, (int)Input::GetMousePositionAbsolute().y);
	ImGui::Text("Worldspace Mouse Position: X: %f, Y: %f, Z: %f", Input::GetMousePositionWorld().x, Input::GetMousePositionWorld().y, Input::GetMousePositionWorld().z);
	ImGui::Text("Run Time: %.2i : %.2i", (int)glm::floor(ts.GetTimeAsMinutes()), (int)glm::floor(ts.GetTimeAsSeconds() - glm::floor(ts.GetTimeAsMinutes()) * 60));
	ImGui::Text("Frame Time: %.2f ms", ts.GetMilliseconds());
	ImGui::Text("FPS: %.1f", 1.f / ts.GetSeconds());
	ImGui::Text("Draw Calls: %d", Renderer::drawCalls);
}
