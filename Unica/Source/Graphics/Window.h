// Copyright João Fonseca, All Rights Reserved.

#pragma once

#include "glfw3.h"

class Window
{
public:
    Window() { }
    Window(string WindowTitle, int32 WindowWidth, int32 WindowHeight);
    ~Window();
    
    void Init();
    void Tick();
    
private:
    GLFWwindow* m_Window = nullptr;
    GLFWmonitor* m_Monitor = nullptr;

    string m_WindowTitle = "UnicaEngine";
    int32 m_WindowWidth = 1600;
    int32 m_WindowHeight = 900;
};
