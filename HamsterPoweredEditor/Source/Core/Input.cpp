#include "Input.h"

#include <array>

#include "Raycast.h"
#include "Timer.h"
#include "Window.h"
#include "Rendering/Renderer.h"

bool Input::IsKeyDown(Keyboard::Key key)
{
    return m_KeyboardStates[key];
}

bool Input::WasKeyPressed(Keyboard::Key key)
{
    return m_KeyboardStates[key] && !m_PrevKeyboardStates[key];
}

bool Input::WasKeyReleased(Keyboard::Key key)
{
    return !m_KeyboardStates[key] && m_PrevKeyboardStates[key];
}

bool Input::IsMouseButtonDown(Mouse::Button button)
{
    return m_MouseButtonStates[button];
}

bool Input::WasMouseButtonPressed(Mouse::Button button)
{
    return m_MouseButtonStates[button] && !m_PrevMouseButtonStates[button];
}

bool Input::WasMouseButtonReleased(Mouse::Button button)
{
    return !m_MouseButtonStates[button] && m_PrevMouseButtonStates[button];
}

glm::vec2 Input::GetMousePosition()
{
    return m_MousePosition;
}

glm::vec2 Input::GetLastMousePosition()
{
    return m_PreviousMousePosition;
}

glm::vec2 Input::GetMousePositionAbsolute()
{
    return m_MousePositionAbsolute;
}

glm::vec2 Input::GetMouseDelta()
{
    return m_MouseDelta;
}

glm::vec3 Input::GetMousePositionWorld()
{
    return RaycastMouse().Location;
}

glm::vec2 Input::GetMousePositionInViewport()
{
    glm::vec2 MousePosAbsolute = Input::GetMousePositionAbsolute();
    glm::vec2 viewportLocationAbsolute = Renderer::GetViewportPosition();
    glm::vec2 mousePosInViewport = MousePosAbsolute - viewportLocationAbsolute;

    return mousePosInViewport;
}

Hit Input::RaycastMouse()
{
    
    glm::vec2 MousePosAbsolute = Input::GetMousePositionAbsolute();
    glm::vec2 viewportLocationAbsolute = Renderer::GetViewportPosition();
    glm::vec2 mousePosInViewport = MousePosAbsolute - viewportLocationAbsolute;

    glm::vec3 v0 = Renderer::ScreenToWorldPos(mousePosInViewport, 0.0f);
    glm::vec3 v1 = Renderer::ScreenToWorldPos(mousePosInViewport, 1.0f);

    glm::vec3 dir  = (v1 - v0); 
    dir = glm::normalize(dir);
    

    Hit hit;
    hit.Location = v0;
    Raycast(v0, dir, hit, 10000);

    return hit;
}

float Input::GetMouseWheelDelta()
{
    return m_MouseScrollDelta;
}

void Input::Reset()
{
    m_PrevMouseButtonStates = m_MouseButtonStates;
    m_PrevKeyboardStates = m_KeyboardStates;
    
    
    m_MouseScrollDelta = 0;
    m_PreviousMousePosition = m_MousePosition;
    m_MousePosition = Context->GetMousePosition();
    m_MousePositionAbsolute = Context->GetMousePositionAbsolute();
    m_MouseDelta = m_MousePosition - m_PreviousMousePosition;
    
    
}
