#pragma once
#include <stdexcept>
#include <string>
#include <glad\glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

//Abstraction of glfw window
class Window
{
public:
    Window(std::string title, int width, int height);
    ~Window();
    
    
    int GetWidth() const;
    int GetHeight() const;

    void SetWidth(int width);
    void SetHeight(int height);
    void SetSize(int width, int height);

    void SetCursorMode(int mode);

    glm::vec2 GetMousePosition() const;
    glm::vec2 GetMousePositionAbsolute() const;

    
    int GetPositionX() const;
    int GetPositionY() const;

    void SetMousePosition(double x, double y);
    void SetPositionX(int x);
    void SetPositionY(int y);
    void SetPosition(int x, int y);

    void Maximise();
    void Minimise();
    void Restore();
    
    std::string GetTitle() const;
    void SetTitle(std::string title);

    void PollEvents();
    void SwapBuffers();

    bool IsMaximised() const;
    
    bool IsOpen() const {return !glfwWindowShouldClose(m_Window);}
    
    GLFWwindow* GetWindowCtx() const {return m_Window;}

    void Quit();
    
private:
    
    int m_Width;
    int m_Height;
    int m_PosX;
    int m_PosY;
    std::string m_Title;
    GLFWwindow* m_Window;
    
};
