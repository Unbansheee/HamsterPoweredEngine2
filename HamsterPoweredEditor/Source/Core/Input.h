#pragma once
#include <array>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Raycast.h"

namespace Keyboard
{
    enum Key
    {
        // Based on GLFW key codes
        Space = 32,
        Apostrophe = 39,
        Comma = 44,
        Minus = 45,
        Period = 46,
        Slash = 47,
        Num0 = 48,
        Num1 = 49,
        Num2 = 50,
        Num3 = 51,
        Num4 = 52,
        Num5 = 53,
        Num6 = 54,
        Num7 = 55,
        Num8 = 56,
        Num9 = 57,
        Semicolon = 59,
        Equal = 61,
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        LeftBracket = 91,
        Backslash = 92,
        RightBracket = 93,
        GraveAccent = 96,
        World1 = 161,
        World2 = 162,
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,
        KP0 = 320,
        KP1 = 321,
        KP2 = 322,
        KP3 = 323,
        KP4 = 324,
        KP5 = 325,
        KP6 = 326,
        KP7 = 327,
        KP8 = 328,
        KP9 = 329,
        KPDecimal = 330,
        KPDivide = 331,
        KPMultiply = 332,
        KPSubtract = 333,
        KPAdd = 334,
        KPEnter = 335,
        KPEqual = 336,
        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348,
        KeyCount = Menu
    };
}

namespace Mouse
{
    enum Button
    {
        // Based on GLFW mouse button codes
        Left = 0,
        Right = 1,
        Middle = 2,
        ButtonCount = Middle
    };
}

class Input
{
public:
    ////////////////////////////
    ///Keyboard
    ////////////////////////////
    
    // Get the current status of a key
    static bool IsKeyDown(Keyboard::Key key);
    
    // Check if a key was activated this frame
    static bool WasKeyPressed(Keyboard::Key key);

    // Check if a key was released this frame
    static bool WasKeyReleased(Keyboard::Key key);

    ////////////////////////////
    ///Mouse
    ////////////////////////////
    
    // Get the current status of a mouse button
    static bool IsMouseButtonDown(Mouse::Button button);

    // Check if a mouse button was activated this frame
    static bool WasMouseButtonPressed(Mouse::Button button);

    // Check if a mouse button was released this frame
    static bool WasMouseButtonReleased(Mouse::Button button);

    static glm::vec2 GetMousePosition(); // Get current mouse position relative to the window
    static glm::vec2 GetLastMousePosition();
    static glm::vec2 GetMousePositionAbsolute(); // Get current mouse position relative to the screen
    static glm::vec2 GetMouseDelta();
    static glm::vec3 GetMousePositionWorld(); // Get current mouse position in world space
    static glm::vec2 GetMousePositionInViewport(); // Get current mouse position relative to the viewport
    static Hit RaycastMouse(); // Raycast from the mouse position into the world

    // Returns the distance of the mouse wheel since the last frame. -1 to 1 space
    static float GetMouseWheelDelta();

private:
    friend class Window;
    static inline std::array<bool, Mouse::Button::ButtonCount> m_MouseButtonStates = {false};
    static inline std::array<bool, Mouse::Button::ButtonCount> m_PrevMouseButtonStates = {false};
    
    static inline std::array<bool, Keyboard::Key::KeyCount> m_KeyboardStates = {false};
    static inline std::array<bool, Keyboard::Key::KeyCount> m_PrevKeyboardStates = {false};

    static inline float m_MouseScrollDelta = 0.0f;
    static inline glm::vec2 m_MousePosition = glm::vec2(0.0f);
    static inline glm::vec2 m_MousePositionAbsolute = glm::vec2(0.0f);
    static inline glm::vec2 m_PreviousMousePosition = glm::vec2(0.0f);
    static inline glm::vec2 m_MouseDelta = glm::vec2(0.0f);

    
    
    static inline Window* Context = nullptr;
    static void SetContext(Window* window) { Context = window; }
    static void Reset();
    
    
};
