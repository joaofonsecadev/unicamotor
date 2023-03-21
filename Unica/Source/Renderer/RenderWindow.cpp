// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "RenderWindow.h"

#include <tracy/Tracy.hpp>

#include "UnicaInstance.h"
#include "UnicaMinimal.h"

RenderWindow::RenderWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_GlfwWindow = glfwCreateWindow(
        UnicaSettings::WindowWidth,
        UnicaSettings::WindowHeight,
        UnicaSettings::EngineName.c_str(),
        nullptr, nullptr);

    UNICA_LOG(Log, __FUNCTION__, "Created application window");
}

void RenderWindow::Tick()
{
    UNICA_PROFILE_FUNCTION
    if (glfwWindowShouldClose(m_GlfwWindow))
    {
        UnicaInstance::RequestExit();
    }
    glfwPollEvents();
}

RenderWindow::~RenderWindow()
{
    glfwDestroyWindow(m_GlfwWindow);
    glfwTerminate();

    UNICA_LOG(Log, __FUNCTION__, "Destroyed application window");
}
