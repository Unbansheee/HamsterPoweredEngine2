#include "App.h"

#include <iostream>
#include <ostream>

#include "Font.h"
#include "UI/ImGuiLayer.h"

#include "imgui.h"
#include "Input.h"
#include "Timer.h"
#include "ResourceManagement/Shader.h"
#include "ResourceManagement/Texture.h"
#include "Window.h"
#include "Actors/CameraController.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "UI/ImFileDialog.h"


void App::Begin()
{
	directory = std::filesystem::current_path().string();
	std::cout << directory << std::endl;

	ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void* {
		GLuint tex;
		
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		return (void*)(uint64_t)tex;
	};

	ifd::FileDialog::Instance().DeleteTexture = [](void* tex) -> void {
		GLuint texID = (GLuint)((uintptr_t)tex);
		glDeleteTextures(1, &texID);
	};
	
	//Entry point of App. Initialises GLAD
	gladLoadGL();

	//Create a window context for the app
	window = new Window("Hamster Powered Editor", 1280, 720);
	window->PollEvents();
	
	//Initialise fonts
	Font::LoadFont("Resources/Fonts/Roboto-Regular.ttf", 100);
	Font::LoadFont("Resources/Fonts/Roboto-Bold.ttf", 100);
	Font::LoadFont("Resources/Fonts/Roboto-Italic.ttf", 100);
	Font::LoadFont("Resources/Fonts/ARIAL.TTF", 100);
	Font::LoadFont("Resources/Fonts/VERDANA.TTF", 100);
	Font::LoadFont("Resources/Fonts/Gresta.ttf", 100);
	Font::LoadFont("Resources/Fonts/South Australia.ttf", 100);
	

	
	
	Renderer::Init();
	Renderer::SetClearColor(glm::vec4(0.09f, 0.09f, 0.12f, 1.0f));

	//Initialise editor. At a later point this can be skipped and the window should show the framebuffer.
	//Currently the framebuffer custom set and is only shown in the Viewport widget, so this must be left on for now.
	
	EditorLayer = new ImGuiLayer(window);
	EditorLayer->Begin();
	
	//LoadScene<Scene>();
	LoadScene("Resources/Scenes/Cloth.json");
}


void App::Update()
{
	//Update timestep
	CurrentTime = (float)glfwGetTime();
	float time = (float)glfwGetTime();
	timestep.Update(time - m_LastFrameTime);
	m_LastFrameTime = time;

	if (EditorLayer) EditorLayer->BeginFrame();
	
	//UPDATE SCENE
	
	if (m_currentScene)
	{
		m_currentScene->Update(timestep);

		if (Input::WasKeyPressed(Keyboard::R))
		{
			Shader::ReloadAllShaders();
		}
		
		//RENDER SCENE
		Renderer::BeginScene(*m_currentScene->m_editorCamera->GetCamera());
		m_currentScene->Render();
		Renderer::Render();
		Renderer::EndScene();

		//Destroy queued items as not to break references in the update loop
		m_currentScene->DeferredDestroy();
	}
	
	//UPDATE/RENDER EDITOR
	if (EditorLayer)
	{
		EditorLayer->Update(timestep);
		EditorLayer->EndFrame();
	}
	

	window->SwapBuffers();
	window->PollEvents();
	

	Renderer::DeferredUpdate();

	ImGui::ResetMouseDragDelta(1);
}


void App::Quit()
{
	window->Quit();

}

float App::GetTime()
{

	return CurrentTime;
}

void App::LoadScene(const std::string& path)
{
	//Load a scene from a file
	EditorLayer->m_SelectedActor = nullptr;
	delete m_currentScene;
	m_currentScene = new Scene();
	m_currentScene->DeserializeScene(path);
	m_currentScene->m_filepath = path;
	m_currentScene->Begin();
}

void App::NewScene()
{
	EditorLayer->m_SelectedActor = nullptr;
	delete m_currentScene;
	m_currentScene = new Scene();
	m_currentScene->Begin();
}
