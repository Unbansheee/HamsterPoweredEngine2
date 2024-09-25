#include "ImGuiLayer.h"

#include <iostream>

#include "Core/App.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include <GLFW/glfw3.h>

#include "HierarchyPanel.h"
#include "InspectorPanel.h"
#include "NodeEditor.h"
#include "UIComponent.h"
#include "Viewport.h"
#include "Stats.h"
#include "ThemeUI.h"
#include "ThemeManager.h"
#include "imnodes.h"
#include "RendererSettings.h"

#include "HPImGui.h"


ImGuiLayer::ImGuiLayer(Window* window) : window(window)
{
	//Initialise Imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImNodes::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window->GetWindowCtx(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

ImGuiLayer::~ImGuiLayer()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImNodes::DestroyContext();
	ImGui::DestroyContext();
}

void ImGuiLayer::Begin()
{
	ThemeManager::LoadTheme("Dark");
	ThemeManager::SetDefaultFont("Resources/Fonts/Roboto-Light.ttf");
	CreateWidget<Viewport>();
	CreateWidget<Stats>();
	CreateWidget<HierarchyPanel>();
	CreateWidget<InspectorPanel>();
	CreateWidget<RendererSettings>();
	
}

void ImGuiLayer::BeginFrame()
{
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::EndFrame()
{
	ImGui::Render();
	
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{

		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImGuiLayer::Update(Timestep ts)
{

	//Update stuff
	ImGui::DockSpaceOverViewport();
	int windowPos[2];
	windowPos[0] = window->GetPositionX();
	windowPos[1] = window->GetPositionY();
	
	//TOP MENU BAR
	ImVec4 normalButton = ImGui::GetStyle().Colors[ImGuiCol_Button];
	ImVec4 darkerButton = ImVec4(normalButton.x + 0.07f, normalButton.y + 0.07f, normalButton.z + 0.07f, normalButton.w);
	ImGui::PushStyleColor(ImGuiCol_Button, darkerButton);
	
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 10));
	
	ImVec4 normalColour = ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg];
	ImVec4 darkerColour = ImVec4(normalColour.x - 0.13f, normalColour.y - 0.13f, normalColour.z - 0.13f, normalColour.w);
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, darkerColour);

	
	ImGui::BeginMainMenuBar();
	ImGui::PopStyleVar(1);
	ImGui::PopStyleColor(1);

	//Cache window dimensions
	int windowWidth = window->GetWidth();
	int windowHeight = window->GetHeight();

	//Caulculate possible resizing directions
	bool resize_top = abs(windowPos[1] - ImGui::GetMousePos().y) <= 5;
	bool resize_left = abs(windowPos[0] - ImGui::GetMousePos().x) <= 5;
	bool resize_right = abs(abs(windowPos[0] - ImGui::GetMousePos().x) - windowWidth) <= 5;
	bool resize_bottom = abs(abs(windowPos[1] - ImGui::GetMousePos().y) - windowHeight) <= 5;

	//Set mouse cursor accordingly
	//Note - GLFW does not currently have diagonal mouse resizing icons :(
	if (!resizeInProgress)
	{
		if (resize_left && resize_bottom)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			resizeDir = RESIZE_BOTTOMLEFT;
			wantsToResize = true;
		}
		else if (resize_right && resize_bottom)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			wantsToResize = true;
			resizeDir = RESIZE_BOTTOMRIGHT;
		}
		else if (resize_top && resize_left)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			resizeDir = RESIZE_TOPLEFT;
			wantsToResize = true;
		}
		else if (resize_top && resize_right)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			resizeDir = RESIZE_TOPRIGHT;
			wantsToResize = true;
		}
		else if (resize_left)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
			resizeDir = RESIZE_LEFT;
			wantsToResize = true;
		}
		else if (resize_top)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
			resizeDir = RESIZE_TOP;
			wantsToResize = true;
		}
		else if (resize_right)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
			resizeDir = RESIZE_RIGHT;
			wantsToResize = true;
		}
		else if (resize_bottom)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
			resizeDir = RESIZE_BOTTOM;
			wantsToResize = true;
		}
		else
		{
			wantsToResize = false;
		}
	}
	
	if (wantsToResize && ImGui::IsMouseClicked(0)) resizeInProgress = true;
	if (!ImGui::IsMouseDown(0)) resizeInProgress = false;

	ImGui::GetIO().MouseDragThreshold = 0.1f;
	
	if (resizeInProgress)
	{
		int resizeL = (int)glm::clamp((int)windowWidth - ImGui::GetMouseDragDelta().x, 500.f, 5000.f);
		int resizeU = (int)glm::clamp((int)windowHeight - ImGui::GetMouseDragDelta().y, 500.f, 5000.f);
		int resizeR = (int)glm::clamp((int)windowWidth + ImGui::GetMouseDragDelta().x, 500.f, 5000.f);
		int resizeD = (int)glm::clamp((int)windowHeight + ImGui::GetMouseDragDelta().y, 500.f, 5000.f);

		
		int moveX = resizeL > 500 ? windowPos[0] + (int)ImGui::GetMouseDragDelta().x : windowPos[0];
		int moveY = resizeU > 500 ? windowPos[1] + (int)ImGui::GetMouseDragDelta().y : windowPos[1];
		
		switch (resizeDir)
		{
		case RESIZE_LEFT:
			window->SetWidth(resizeL);
			window->SetPositionX(moveX);
			break;
		case RESIZE_BOTTOM:
			window->SetHeight(resizeD);
			break;
		case RESIZE_TOP:
			window->SetHeight(resizeU);
			window->SetPositionY(moveY);
			break;
		case RESIZE_RIGHT:
			window->SetWidth(resizeR);
			break;
		case RESIZE_TOPLEFT:
			window->SetSize(resizeL, resizeU);
			window->SetPosition(moveX, moveY);
			break;
		case RESIZE_TOPRIGHT:
			window->SetSize(resizeR, resizeU);
			window->SetPositionY(moveY);
			break;
		case RESIZE_BOTTOMLEFT:
			window->SetSize(resizeL, resizeD);
			window->SetPositionX(moveX);
			break;
		case RESIZE_BOTTOMRIGHT:
			window->SetSize(resizeR, resizeD);
			break;
		}
	}
	ImGui::ResetMouseDragDelta();
	
	if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() && !resizeInProgress)
	{
		if (ImGui::IsMouseClicked(0))
		{

			//get offset of mouse from window
			mouseRel[0] = (int)ImGui::GetMousePos().x - window->GetPositionX();
			mouseRel[1] = (int)ImGui::GetMousePos().y - window->GetPositionY();
			
			windowDragged = true;

			
		}
	}
	
	if (windowDragged)
	{
		if (ImGui::IsMouseDown(0))
		{
			//Drag window
			window->SetPosition((int)ImGui::GetMousePos().x - mouseRel[0], (int)ImGui::GetMousePos().y - mouseRel[1]);
			//UnMaximise window if it is maximised and gets dragged
			if (window->IsMaximised() && ImGui::IsMouseDragging(0))
			{
				float xPercent = (ImGui::GetMousePos().x - (float)window->GetPositionX()) / (float)window->GetWidth();
				
				window->Restore();
				
				window->SetPosition((int)(ImGui::GetMousePos().x - (float)window->GetWidth() * xPercent), (int)ImGui::GetMousePos().y - 10);
				
				mouseRel[0] = (int)ImGui::GetMousePos().x - window->GetPositionX();
				mouseRel[1] = (int)ImGui::GetMousePos().y - window->GetPositionY();
			}
		}
		else
		{
			//Maximise the window if it gets dropped at the top of the screen
			if (ImGui::IsMouseReleased(0) && windowDragged)
			{
				windowDragged = false;
				if (ImGui::GetMousePos().y < 5)
				{
					window->Maximise();
				}
			}
		}
	}


	
	ImGui::Text("Hamster Powered Editor");

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
	ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 95);
	if (ImGui::Button("_", ImVec2(33, ImGui::GetContentRegionMax().y)))
	{
		window->Minimise();
	}
	ImGui::SameLine(0, 0);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
	if (ImGui::Button("O", ImVec2(33, ImGui::GetContentRegionMax().y)))
	{
		if (glfwGetWindowAttrib(App::Instance().window->GetWindowCtx(), GLFW_MAXIMIZED))
		{
			window->Restore();
		}
		else
		{
			window->Maximise();
		}
	}
	ImGui::SameLine(0, 0);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.1f, 0.1f, 1.0f));
	if (ImGui::Button("X", ImVec2(33, ImGui::GetContentRegionMax().y)))
	{
		
		App::Instance().Quit();
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	
	ImGui::EndMainMenuBar();
	
	
	
	ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
	float height = ImGui::GetFrameHeight();

	//Code for a secondary menu bar
	
	if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, height, window_flags)) {
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					App::Instance().NewScene();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					Load();
				}
				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					Save();
				}
				if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
				{
					SaveAs();
					
				}

				ImGui::Separator();
				if (ImGui::MenuItem("Exit"))
				{
					App::Instance().Quit();
				}
				ImGui::EndMenu();
			}

			//Edit Menu
			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::EndMenu();
			}

			//Window Menu
			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::MenuItem("Viewport"))
				{
					CreateWidget<Viewport>();
				}
				if (ImGui::MenuItem("Stats"))
				{
					CreateWidget<Stats>();
				}
				if(ImGui::MenuItem("Hierarchy"))
				{
					CreateWidget<HierarchyPanel>();
				}
				if (ImGui::MenuItem("Inspector"))
				{
					CreateWidget<InspectorPanel>();
				}
				if (ImGui::MenuItem("Node Editor"))
				{
					CreateWidget<NodeEditor>();
				}
				if (ImGui::MenuItem("Themes"))
				{
					CreateWidget<ThemeUI>();
				}
				ImGui::EndMenu();
			}
			
			ImGui::EndMenuBar();

			if (ifd::FileDialog::Instance().IsDone("OpenScene"))
			{
				if (ifd::FileDialog::Instance().HasResult()) {
					std::string result = ifd::FileDialog::Instance().GetResult().u8string();
					std::replace(result.begin(), result.end(), '\\', '/');
					App::Instance().LoadScene(result);
				}
				ifd::FileDialog::Instance().Close();
			}

			if (ifd::FileDialog::Instance().IsDone("SaveScene"))
			{
				if (ifd::FileDialog::Instance().HasResult()) {
					std::string result = ifd::FileDialog::Instance().GetResult().u8string();
					std::replace(result.begin(), result.end(), '\\', '/');
					App::Instance().m_currentScene->SerializeScene(result);
					App::Instance().m_currentScene->m_filepath = result;
				}
				ifd::FileDialog::Instance().Close();
			}
			
		}
		ImGui::End();
	}
	


	if (ImGui::BeginViewportSideBar("##MainStatusBar", viewport, ImGuiDir_Down, height, window_flags)) {
		if (ImGui::BeginMenuBar()) {
			std::string status = "Scene: " + App::Instance().m_currentScene->GetPath() + " | FPS: " + std::to_string(1000.f / App::Instance().timestep.GetMilliseconds());
			ImGui::Text(status.c_str());
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	for (int i = 0; i < UIComponents.size(); i++)
	{
		auto item = UIComponents[i];
		if (item && item->isOpen)
		{
			item->Begin();
			item->Update(ts);
			item->End();
		}
		else
		{
			delete UIComponents[i];
			UIComponents.erase(UIComponents.begin() + i);
		}
		
	}



}

void ImGuiLayer::SaveAs()
{
	ifd::FileDialog::Instance().Save("SaveScene", "Save Scene", "Scene (*.json;*.scene){.json,.scene}", "Resources/Scenes");
}

void ImGuiLayer::Save()
{
	if (App::Instance().m_currentScene->m_filepath.empty()) SaveAs();
	else App::Instance().m_currentScene->SerializeScene(App::Instance().m_currentScene->m_filepath);
}

void ImGuiLayer::Load()
{
	ifd::FileDialog::Instance().Open("OpenScene", "Open Scene", "Scene (*.json;*.scene){.json,.scene}", false, "Resources/Scenes");
}
