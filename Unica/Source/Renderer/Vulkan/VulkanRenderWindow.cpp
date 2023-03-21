// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "VulkanRenderWindow.h"

#include <tracy/Tracy.hpp>

#include "UnicaInstance.h"
#include "UnicaMinimal.h"

GlfwRenderWindow::GlfwRenderWindow()
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

void GlfwRenderWindow::Tick()
{
    UNICA_PROFILE_FUNCTION
    WindowCloseRequested();
    PoolEvents();
}

void GlfwRenderWindow::WindowCloseRequested()
{
    UNICA_PROFILE_FUNCTION
    
    bool glfwWindowCloseRequest;
    {
        UNICA_PROFILE_FUNCTION_NAMED("glfw::glfwWindowShouldClose")
        glfwWindowCloseRequest = glfwWindowShouldClose(m_GlfwWindow);
    }
    
    if (glfwWindowCloseRequest)
    {
        UnicaInstance::RequestExit();
    }
}

void GlfwRenderWindow::PoolEvents()
{
    UNICA_PROFILE_FUNCTION
    {
        UNICA_PROFILE_FUNCTION_NAMED("glfw::glfwPollEvents")
        glfwPollEvents();
    }
}

GlfwRenderWindow::~GlfwRenderWindow()
{
    glfwDestroyWindow(m_GlfwWindow);
    glfwTerminate();

    UNICA_LOG(Log, __FUNCTION__, "Destroyed application window");
}