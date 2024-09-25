#include "Window.h"

#include <iostream>

#include "Input.h"
#include "Timer.h"
#include <array>

Window::Window(std::string title, int _width, int _height) : m_Title(title), m_Width(_width), m_Height(_height)
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    //Removes default Windows decoration. Why did I decide to do this...
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    

    //Instantiate glfw window
    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
    if (!m_Window)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    //Centers the window on the main monitor
    int monitor[4];
    glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monitor[0], &monitor[1], &monitor[2], &monitor[3]);
    glfwSetWindowPos(m_Window, monitor[0] + (monitor[2] - m_Width) / 2, monitor[1] + (monitor[3] - m_Height) / 2);
    glfwGetWindowPos(m_Window, &m_PosX, &m_PosY);
    
    glfwMakeContextCurrent(m_Window);


    
    //Unlimit the framerate
    glfwSwapInterval(0);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            Input::m_KeyboardStates[key] = action;
        });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            Input::m_MouseButtonStates[button] = action;
        });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            Input::m_MouseScrollDelta = yoffset;
        });
    
    Input::SetContext(this);

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

int Window::GetWidth() const
{
    return m_Width;
}

int Window::GetHeight() const
{
    return m_Height;
}

int Window::GetPositionX() const
{
    return m_PosX;
}

int Window::GetPositionY() const
{
    return m_PosY;
}

void Window::SetMousePosition(double x, double y)
{
    glfwSetCursorPos(m_Window, x, y);
}

void Window::SetPositionX(int x)
{
    glfwSetWindowPos(m_Window, x, m_PosY);
    m_PosX = x;
}

void Window::SetPositionY(int y)
{
    glfwSetWindowPos(m_Window, m_PosX, y);
    m_PosY = y;

}

void Window::SetPosition(int x, int y)
{
    glfwSetWindowPos(m_Window, x, y);
    m_PosX = x;
    m_PosY = y;
}

void Window::Maximise()
{
    glfwMaximizeWindow(m_Window);
    glfwGetWindowSize(m_Window, &m_Width, &m_Height);
    glfwGetWindowPos(m_Window, &m_PosX, &m_PosY);
}

void Window::Minimise()
{
    glfwIconifyWindow(m_Window);
    glfwGetWindowSize(m_Window, &m_Width, &m_Height);
    glfwGetWindowPos(m_Window, &m_PosX, &m_PosY);
}

void Window::Restore()
{
    glfwRestoreWindow(m_Window);
    glfwGetWindowSize(m_Window, &m_Width, &m_Height);
    glfwGetWindowPos(m_Window, &m_PosX, &m_PosY);
}

std::string Window::GetTitle() const
{
    return m_Title;
}

void Window::SetWidth(int width)
{
    m_Width = width;
    glfwSetWindowSize(m_Window, m_Width, m_Height);
    glfwGetWindowSize(m_Window, &m_Width, &m_Height);
}

void Window::SetHeight(int height)
{
    m_Height = height;
    glfwSetWindowSize(m_Window, m_Width, m_Height);
    glfwGetWindowSize(m_Window, &m_Width, &m_Height);
    
}

void Window::SetSize(int width, int height)
{
    m_Width = width;
    m_Height = height;
    glfwSetWindowSize(m_Window, m_Width, m_Height);
    glfwGetWindowSize(m_Window, &m_Width, &m_Height);
}

void Window::SetCursorMode(int mode)
{
    glfwSetInputMode(m_Window, GLFW_CURSOR, mode);
}

glm::vec2 Window::GetMousePosition() const
{
    double x, y;
    glfwGetCursorPos(m_Window, &x, &y);
    return glm::vec2(x, y);
}

glm::vec2 Window::GetMousePositionAbsolute() const
{
    double x, y;
    glfwGetCursorPos(m_Window, &x, &y);
    return glm::vec2(x + m_PosX, y + m_PosY);
    
}


void Window::SetTitle(std::string title)
{
    m_Title = title;
    glfwSetWindowTitle(m_Window, m_Title.c_str());
    
}

void Window::PollEvents()
{
    Input::Reset();
    glfwPollEvents();
    
}


void Window::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
    
}

bool Window::IsMaximised() const
{
    return glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
}  

void Window::Quit()
{
    glfwSetWindowShouldClose(m_Window, GL_TRUE);
}
