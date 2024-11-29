#pragma once

#include <vector>
#include <tracy/Tracy.hpp>

#include "vulkanallocatedimage.h"
#include "vulkanframedata.h"
#include "vulkan/vulkan_core.h"
#include "renderer/renderer.h"

struct GLFWwindow;
struct VmaAllocator_T;

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
    void DrawFrame();
    void DrawBackground(VkCommandBuffer command_buffer);


    bool CreateWindow();
    bool CreateVulkanInstance();
    bool CreateVulkanSwapchain(const uint16_t extent_width, const uint16_t extent_height);
    bool CreateCommandPools();
    bool CreateSyncStructures();

    bool DestroySwapchain();
    bool DestroyCommandPoolFenceAndSemaphores();

    VulkanFrameData& GetCurrentFrameData() { return m_frame_data.at(GetCurrentFrameIndex()); }
    VulkanFrameData& GetFrameData(const uint8_t frame_index) { ZoneScoped; return m_frame_data[frame_index]; }
    [[nodiscard]] uint8_t GetCurrentFrameIndex() const { ZoneScoped; return m_frame_count % m_frame_buffer_amount; }

    static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugMessenger(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void*);

    // GLFW
    GLFWwindow* m_glfw_window = nullptr;

    // Vulkan Instance
    uint64_t m_frame_count = 0;
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
    uint8_t m_frame_buffer_amount = -1;

    VulkanAllocatedImage m_draw_image;
    VkExtent2D m_draw_extent = { 0, 0 };

    // Vulkan Queues and Commands
    VulkanDeletionQueue m_global_deletion_queue;
    std::vector<VulkanFrameData> m_frame_data;
    VkQueue m_graphics_queue = nullptr; // @TODO using a single queue, ideally multiple for different work (vkb::QueueType)
    uint32_t m_graphics_queue_family_index = -1;

    // Vulkan Memory Allocation
    VmaAllocator_T* m_memory_allocator = nullptr;
};