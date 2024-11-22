#pragma once

#include <vector>
#include "vulkan/vulkan_core.h"
#include "renderer/renderer.h"

struct GLFWwindow;

enum VkFormat : int32_t;

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
    bool CreateVulkanSwapchain(const uint16_t extent_width, const uint16_t extent_height);

    bool DestroySwapchain();

    static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugMessenger(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void*);

    // GLFW
    GLFWwindow* m_glfw_window = nullptr;

    // Vulkan Instance
    VkInstance m_vulkan_instance = nullptr;
    VkSurfaceKHR m_glfw_window_surface = nullptr;
    VkDebugUtilsMessengerEXT m_vulkan_messenger = nullptr;

    // Vulkan Devices
    VkDevice m_vulkan_device = nullptr;
    VkPhysicalDevice m_physical_device = nullptr;

    // Vulkan Swapchain
    VkSwapchainKHR m_swapchain = nullptr;
    VkFormat m_swapchain_image_format = VK_FORMAT_UNDEFINED;
    std::vector<VkImage> m_swapchain_images;
    std::vector<VkImageView> m_swapchain_image_views;
    VkExtent2D m_swapchain_extent = { 0, 0 };
};