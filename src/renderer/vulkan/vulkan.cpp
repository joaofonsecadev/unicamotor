#include "vulkan.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "core/directories.h"
#include <toml++/toml.hpp>

#include "core/unicamotor.h"
#include "spdlog/spdlog.h"


RendererVulkan::RendererVulkan(Unicamotor* engine) : RendererSubsystem(engine)
{

}

bool RendererVulkan::Init()
{
    CreateWindow();
    return true;
}

void RendererVulkan::Tick()
{
    glfwPollEvents();
    if (glfwWindowShouldClose(m_window))
    {
        Unicamotor::RequestExit();
    }
}

bool RendererVulkan::CreateWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    toml::parse_result engine_config = toml::parse_file(DirectoriesHelper::EngineDefaultConfigDirectory().c_str());
    m_window = glfwCreateWindow(engine_config["GRAPHICS"]["RESOLUTION_WIDTH"].value_or(1600), engine_config["GRAPHICS"]["RESOLUTION_HEIGHT"].value_or(900), engine_config["CORE"]["NAME"].value_or("UNICAMOTOR DEFAULT"), nullptr, nullptr);
    if (!m_window)
    {
        return false;
    }

    return true;
}

RendererVulkan::~RendererVulkan()
{

}
