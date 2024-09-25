#pragma once
#include <memory>
#include <vector>


#include "Rendering/OrthographicCamera.h"
#include "Rendering/Renderer.h"
//#include "Window.h"
#include "Timestep.h"
#include "Rendering/GLFrameBuffer.h"
#include "Actors/CameraController.h"
#include "Core/Scene.h"

struct GLFWwindow;
class Window;
class Scene;

class ImGuiLayer;
class App
{
public:
	void Update();
	void Begin();
	void Quit();
	float GetTime();

	static std::string GetDirectory() {return directory;};
	
	template<typename T>
	void LoadScene()
		{
			if (m_currentScene != nullptr)
			{
				delete m_currentScene;
				m_currentScene = nullptr;
			}
			m_currentScene = new T();
			m_currentScene->Begin();
		}

	void LoadScene(const std::string& path);
	void NewScene();
	
private:
	App(){}

	float CurrentTime;

	
	friend class Quad;
	friend class HierarchyPanel;
public:
	static App& Instance()
	{
		static App instance; // Guaranteed to be destroyed.
							  // Instantiated on first use.
		return instance;
	}
	
	App(App const&) = delete;
	void operator=(App const&) = delete;

	ImGuiLayer* EditorLayer = nullptr;
	Window* window = nullptr;
	Timestep timestep;
	float m_LastFrameTime = 0.0f;

	Scene* m_currentScene;
	std::shared_ptr<GLFrameBuffer> m_FrameBuffer;
	inline static std::string directory;
	
	
};

