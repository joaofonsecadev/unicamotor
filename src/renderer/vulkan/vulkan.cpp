#define VMA_IMPLEMENTATION
#include "vulkan.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "toml++/toml.hpp"
#include "spdlog/spdlog.h"
#include "VkBootstrap.h"

#include "core/unicamotor.h"
#include "core/directories.h"


RendererVulkan::RendererVulkan(Unicamotor* engine) : RendererSubsystem(engine)
{

}

bool RendererVulkan::Init()
{
    if (CreateWindow()
        && CreateVulkanInstance())
    {
        return true;
    }
    Unicamotor::RequestExit();
    return false;
}

void RendererVulkan::Tick()
{
    glfwPollEvents();
    if (glfwWindowShouldClose(m_glfw_window))
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
    m_glfw_window = glfwCreateWindow(engine_config["GRAPHICS"]["RESOLUTION_WIDTH"].value_or(1600), engine_config["GRAPHICS"]["RESOLUTION_HEIGHT"].value_or(900), engine_config["CORE"]["NAME"].value_or("UNICAMOTOR DEFAULT"), nullptr, nullptr);
    if (!m_glfw_window)
    {
        SPDLOG_CRITICAL("Failed to create a window");
        return false;
    }

    return true;
}

bool RendererVulkan::CreateVulkanInstance()
{
    toml::parse_result engine_config = toml::parse_file(DirectoriesHelper::EngineDefaultConfigDirectory().c_str());

    vkb::InstanceBuilder vulkan_builder;
    vkb::Result<vkb::Instance> vulkan_instance_build_result = vulkan_builder
        .request_validation_layers(engine_config["GRAPHICS"]["VULKAN_VALIDATION_LAYERS_ENABLED"].value_or(false))
        .use_default_debug_messenger()
        .require_api_version(VK_API_VERSION_1_3)
        .build();

    if (!vulkan_instance_build_result.has_value())
    {
        SPDLOG_CRITICAL("Failed to create a Vulkan instance with error code {}", static_cast<uint64_t>(vulkan_instance_build_result.vk_result()));
        return false;
    }

    m_vulkan_instance = vulkan_instance_build_result->instance;
    m_vulkan_messenger = vulkan_instance_build_result->debug_messenger;

    if (glfwCreateWindowSurface(m_vulkan_instance, m_glfw_window, nullptr, &m_glfw_window_surface) != VK_SUCCESS)
    {
        SPDLOG_CRITICAL("Cannot create a vulkan surface in the glfw window");
        return false;
    }

    //vulkan 1.3 features
    VkPhysicalDeviceVulkan13Features features { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
    features.dynamicRendering = true;
    features.synchronization2 = true;

    //vulkan 1.2 features
    VkPhysicalDeviceVulkan12Features features12 { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
    features12.bufferDeviceAddress = true;
    features12.descriptorIndexing = true;

    vkb::PhysicalDeviceSelector vkbootstrap_physical_device_selector { vulkan_instance_build_result.value() };
    vkb::PhysicalDevice vkbootstrap_physical_device = vkbootstrap_physical_device_selector
        .set_minimum_version(1, 3)
        .set_required_features_13(features)
        .set_required_features_12(features12)
        .set_surface(m_glfw_window_surface)
        .select()
        .value();

    vkb::DeviceBuilder vkbootstrap_device_builder { vkbootstrap_physical_device };
    vkb::Device vkbootstrap_device = vkbootstrap_device_builder.build().value();
    m_vulkan_device = vkbootstrap_device.device;
    m_physical_device = vkbootstrap_physical_device.physical_device;

    return true;
}

RendererVulkan::~RendererVulkan()
{
    vkDestroySurfaceKHR(m_vulkan_instance, m_glfw_window_surface, nullptr);
    vkDestroyDevice(m_vulkan_device, nullptr);

    vkb::destroy_debug_utils_messenger(m_vulkan_instance, m_vulkan_messenger);
    vkDestroyInstance(m_vulkan_instance, nullptr);

    glfwDestroyWindow(m_glfw_window);
    glfwTerminate();
}
