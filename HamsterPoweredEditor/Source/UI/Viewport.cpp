#include "Viewport.h"

#include <iostream>

#include "Core/App.h"
#include "Core/Scene.h"
#include "Core/Window.h"
#include "imgui_internal.h"
#include "Core/Input.h"

void Viewport::Init()
{
	name = "Viewport";
	canBeClosed = true;

}

void Viewport::Update(Timestep ts)
{
	
	CameraController* cam = App::Instance().m_currentScene->m_editorCamera;
	ImVec2 cursor = ImGui::GetWindowPos();
	ImGui::PopStyleVar(1);
	ImGui::Image((void*)(uint64_t)Renderer::m_FrameBuffer->GetFinalFrameColorID(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

	//check if resized
	if (ImGui::GetWindowSize().x != lastSize.x || ImGui::GetWindowSize().y != lastSize.y)
	{
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		
		float width = vMax.x - vMin.x;
		float height = vMax.y - vMin.y;
		Renderer::Resize((int)width, (int)height);
		App::Instance().m_currentScene->m_editorCamera->Resize(width, height);
	}
	lastSize = ImGui::GetWindowSize();

	if (cursor.x != lastPos.x || cursor.y != lastPos.y)
	{
		// get imgui titlebar height
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();

		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;
		
		cursor.x = vMin.x;
		cursor.y = vMin.y;
		
		float x = cursor.x;
		float y = cursor.y;
		Renderer::m_viewportPosition = { x, y };
	}
	lastPos = cursor;

	if (Input::WasMouseButtonReleased(Mouse::Right))
	{
		mouseCaptured = false;
		App::Instance().window->SetCursorMode(GLFW_CURSOR_NORMAL);
	}
	
	if (ImGui::IsItemHovered())
	{
		if (Input::WasMouseButtonPressed(Mouse::Right))
		{
			mouseCaptured = true;

		}

		if (cam->GetCameraType() == CameraController::CameraType::ORTHO) cam->SetZoom(cam->GetZoom() - (Input::GetMouseWheelDelta()/4.f) * cam->GetZoom()/2.f);
		
	}
	if (mouseCaptured)
	{
		if (ImGui::IsMouseClicked(1)) return;

		cam->HandleMouseMovement(-Input::GetMouseDelta().x, Input::GetMouseDelta().y);
		App::Instance().window->SetCursorMode(GLFW_CURSOR_DISABLED);

		
		
		
		if (cam->GetCameraType() == CameraController::CameraType::ORTHO) cam->SetZoom(cam->GetZoom() - (Input::GetMouseWheelDelta()/4.f) * cam->GetZoom()/2.f);
		else cam->SetPerspSpeed(cam->GetPerspSpeed() + (Input::GetMouseWheelDelta()/4.f) * cam->GetPerspSpeed()/2.f);

		if (Input::IsKeyDown(Keyboard::W))
		{
			cam->Move({1, 0, 0});
		}
		if (Input::IsKeyDown(Keyboard::S))
		{
			cam->Move({ -1, 0, 0 });
		}
		if (Input::IsKeyDown(Keyboard::A))
		{
			cam->Move({ 0, -1, 0 });
		}
		if (Input::IsKeyDown(Keyboard::D))
		{
			cam->Move({ 0, 1, 0 });
		}
		if (Input::IsKeyDown(Keyboard::Q))
		{
			cam->Move({ 0, 0, 1 });
		}
		if (Input::IsKeyDown(Keyboard::E))
		{
			cam->Move({ 0, 0, -1 });
		}
	}
	
}

void Viewport::Begin()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2, 2));
	UIComponent::Begin();
}

void Viewport::End()
{
	UIComponent::End();
}
