#pragma once


#include "renderer/renderer.h"

struct GLFWwindow;
struct VkInstance_T;
struct VkSurfaceKHR_T;
struct VkDebugUtilsMessengerEXT_T;
struct VkDevice_T;
struct VkPhysicalDevice_T;

class RendererVulkan : public RendererSubsystem
{
public:
    RendererVulkan() = delete;
    explicit RendererVulkan(Unicamotor* engine);
    ~RendererVulkan() override;

    bool Init() override;
    void Tick() override;

private:
    bool CreateWindow();
    bool CreateVulkanInstance();

    // GLFW
    GLFWwindow* m_glfw_window = nullptr;
    VkSurfaceKHR_T* m_glfw_window_surface = nullptr;

    // Vulkan
    VkInstance_T* m_vulkan_instance = nullptr;
    VkDebugUtilsMessengerEXT_T* m_vulkan_messenger = nullptr;
    VkDevice_T* m_vulkan_device = nullptr;
    VkPhysicalDevice_T* m_physical_device = nullptr;
};