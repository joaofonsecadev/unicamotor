#include "vulkan.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

#include "toml++/toml.hpp"
#include "spdlog/spdlog.h"
#include "VkBootstrap.h"
#include "vulkaninitializers.h"
#include "vulkanutilities.h"

#include "core/unicamotor.h"
#include "core/directories.h"

constexpr uint64_t gpu_sync_timeout = 3'000'000'000;

RendererVulkan::RendererVulkan(Unicamotor* engine) : RendererSubsystem(engine)
{

}

bool RendererVulkan::Init()
{
    toml::parse_result engine_config = toml::parse_file(DirectoriesHelper::EngineDefaultConfigDirectory().c_str());

    if (CreateWindow()
        && CreateVulkanInstance()
        && CreateVulkanSwapchain(engine_config["GRAPHICS"]["RESOLUTION_WIDTH"].value_or(1280), engine_config["GRAPHICS"]["RESOLUTION_HEIGHT"].value_or(720))
        && CreateCommandPools()
        && CreateSyncStructures())
    {
        return true;
    }
    Unicamotor::RequestExit();
    return false;
}

void RendererVulkan::Tick()
{
    ZoneScoped;

    {
        ZoneScopedN("glfw::glfwPollEvents");
        glfwPollEvents();
    }

    bool should_window_close = false;
    {
        ZoneScopedN("glfw::glfwWindowShouldClose");
        should_window_close = glfwWindowShouldClose(m_glfw_window);
    }

    if (should_window_close)
    {
        Unicamotor::RequestExit();
    }

    DrawFrame();
}

void RendererVulkan::DrawFrame()
{
    ZoneScoped;

    uint8_t current_frame_index = GetCurrentFrameIndex();
    VulkanFrameData& current_frame_data = GetFrameData(current_frame_index);

    {
        ZoneScopedN("vulkan::vkWaitForFences");
        if (vkWaitForFences(m_vulkan_device, 1, &current_frame_data.render_fence, true, gpu_sync_timeout) != VK_SUCCESS)
        {
            SPDLOG_CRITICAL("Failure on frame index {} while trying to wait for frame render fence on", current_frame_index);
            Unicamotor::RequestExit();
            return;
        }
    }

    current_frame_data.deletion_queue.FlushDeletionQueue();

    {
        ZoneScopedN("vulkan::vkResetFences");
        if (vkResetFences(m_vulkan_device, 1, &current_frame_data.render_fence) != VK_SUCCESS)
        {
            SPDLOG_CRITICAL("Failure on frame index {} resetting frame render fence", current_frame_index);
            Unicamotor::RequestExit();
            return;
        }
    }

    uint32_t swapchain_image_index;
    {
        ZoneScopedN("vulkan::vkAcquireNextImageKHR");
        if (vkAcquireNextImageKHR(m_vulkan_device, m_swapchain, gpu_sync_timeout, current_frame_data.swapchain_semaphore, nullptr, &swapchain_image_index) != VK_SUCCESS)
        {
            SPDLOG_CRITICAL("Failure on frame index {} acquirying next swapchain image", current_frame_index);
            Unicamotor::RequestExit();
            return;
        }
    }

    VkCommandBuffer command_buffer = current_frame_data.main_command_buffer;
    {
        ZoneScopedN("vulkan::vkResetCommandBuffer");
        if (vkResetCommandBuffer(command_buffer, 0) != VK_SUCCESS)
        {
            SPDLOG_CRITICAL("Failure on frame index {} resetting command buffer", current_frame_index);
            Unicamotor::RequestExit();
            return;
        }
    }

    VkCommandBufferBeginInfo command_buffer_begin_info  = VulkanInitializers::CommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    {
        ZoneScopedN("vulkan::vkBeginCommandBuffer");
        if (vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info) != VK_SUCCESS)
        {
            SPDLOG_CRITICAL("Failure on frame index {} beginning the command buffer", current_frame_index);
            Unicamotor::RequestExit();
            return;
        }
    }

    {
        ZoneScopedN("RendererVulkan::DrawFrame::CommandBufferFill");
        VulkanUtilities::TransitionImage(command_buffer, m_swapchain_images[current_frame_index], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);

        VkClearColorValue clear_color_value;
        const float flash = std::abs(std::sin(static_cast<float>(m_frame_count) / 120.f));
        clear_color_value = { { 0.0f, 0.0f, flash, 1.0f } };

        VkImageSubresourceRange clear_image_range = VulkanInitializers::ImageSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT);

        vkCmdClearColorImage(command_buffer, m_swapchain_images[current_frame_index], VK_IMAGE_LAYOUT_GENERAL, &clear_color_value, 1, &clear_image_range);

        VulkanUtilities::TransitionImage(command_buffer, m_swapchain_images[current_frame_index],VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    }


    {
        ZoneScopedN("vulkan::vkEndCommandBuffer");
        if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS)
        {
            SPDLOG_CRITICAL("Failure on frame index {} ending the command buffer", current_frame_index);
            Unicamotor::RequestExit();
            return;
        }
    }


    {
        ZoneScopedN("vulkan::vkQueueSubmit2");
        VkCommandBufferSubmitInfo command_buffer_submit_info = VulkanInitializers::CommandBufferSubmitInfo(command_buffer);

        VkSemaphoreSubmitInfo wait_semaphore_submit_info = VulkanInitializers::SemaphoreSubmitInfo(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR, current_frame_data.swapchain_semaphore);
        VkSemaphoreSubmitInfo signal_semaphore_submit_info = VulkanInitializers::SemaphoreSubmitInfo(VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, current_frame_data.render_semaphore);

        VkSubmitInfo2 submit_info = VulkanInitializers::SubmitInfo(&command_buffer_submit_info, &signal_semaphore_submit_info, &wait_semaphore_submit_info);

        if (vkQueueSubmit2(m_graphics_queue, 1, &submit_info, current_frame_data.render_fence) != VK_SUCCESS)
        {
            SPDLOG_CRITICAL("Failure on frame index {} while submitting to the queue", current_frame_index);
            Unicamotor::RequestExit();
            return;
        }
    }

    {
        ZoneScopedN("vulkan::vkQueuePresentKHR");
        VkPresentInfoKHR present_info = {};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.pNext = nullptr;
        present_info.pSwapchains = &m_swapchain;
        present_info.swapchainCount = 1;
        present_info.pWaitSemaphores = &current_frame_data.render_semaphore;
        present_info.waitSemaphoreCount = 1;
        present_info.pImageIndices = &swapchain_image_index;

        if (vkQueuePresentKHR(m_graphics_queue, &present_info) != VK_SUCCESS)
        {
            SPDLOG_CRITICAL("Failure on frame index {} when presenting the frame", current_frame_index);
            Unicamotor::RequestExit();
            return;
        }
    }

    ++m_frame_count;
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
        .set_debug_callback(VulkanDebugMessenger)
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

    m_graphics_queue = vkbootstrap_device.get_queue(vkb::QueueType::graphics).value();
    m_graphics_queue_family_index = vkbootstrap_device.get_queue_index(vkb::QueueType::graphics).value();

    VmaAllocatorCreateInfo allocator_create_info = { };
    allocator_create_info.physicalDevice = m_physical_device;
    allocator_create_info.device = m_vulkan_device;
    allocator_create_info.instance = m_vulkan_instance;
    allocator_create_info.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
    vmaCreateAllocator(&allocator_create_info, &m_memory_allocator);

    m_global_deletion_queue.PushDeletionFunction([&]
    {
        vmaDestroyAllocator(m_memory_allocator);
    });

    return true;
}

bool RendererVulkan::CreateVulkanSwapchain(const uint16_t extent_width, const uint16_t extent_height)
{
    vkb::SwapchainBuilder swapchain_builder{ m_physical_device, m_vulkan_device, m_glfw_window_surface };
    m_swapchain_image_format = VK_FORMAT_B8G8R8A8_UNORM;

    toml::parse_result engine_config = toml::parse_file(DirectoriesHelper::EngineDefaultConfigDirectory().c_str());
    const bool vsync_enabled = engine_config["GRAPHICS"]["VSYNC"].value_or(true);
    if (!vsync_enabled)
    {
        SPDLOG_WARN("Enabling VSync even though it's turned off in the settings (TODO)");
    }

    vkb::Result<vkb::Swapchain> swapchain_builder_result = swapchain_builder
        .set_desired_format(VkSurfaceFormatKHR{ .format = m_swapchain_image_format, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
        // @TODO Settings for vsync on or off, currently FORCED VSYNC
        .set_desired_present_mode(VK_PRESENT_MODE_FIFO_RELAXED_KHR)
        .set_desired_extent(extent_width, extent_height)
        .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
        .build();

    if (!swapchain_builder_result.has_value())
    {
        SPDLOG_CRITICAL("Swapchain creation failed, {}", static_cast<uint64_t>(swapchain_builder_result.vk_result()));
        return false;
    }

    vkb::Swapchain vkbootstrap_swapchain = swapchain_builder_result.value();
    m_swapchain_extent = vkbootstrap_swapchain.extent;
    m_swapchain = vkbootstrap_swapchain.swapchain;
    m_swapchain_images = vkbootstrap_swapchain.get_images().value();
    m_swapchain_image_views = vkbootstrap_swapchain.get_image_views().value();

    m_frame_buffer_amount = m_swapchain_images.size();
    m_frame_data.resize(m_frame_buffer_amount);
    return true;
}

bool RendererVulkan::CreateCommandPools()
{
    VkCommandPoolCreateInfo command_pool_info = VulkanInitializers::CommandPoolCreateInfo(m_graphics_queue_family_index, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    for (uint8_t i = 0; i < m_frame_buffer_amount; i++)
    {
        VulkanFrameData& frame_data = m_frame_data.at(i);
        if (vkCreateCommandPool(m_vulkan_device, &command_pool_info, nullptr, &frame_data.command_pool) != VK_SUCCESS)
        {
            SPDLOG_CRITICAL("Failed to create command pool for image {}", i);
            return false;
        }

        VkCommandBufferAllocateInfo command_buffer_allocate_info = VulkanInitializers::CommandBufferAllocateInfo(frame_data.command_pool);
        if (vkAllocateCommandBuffers(m_vulkan_device, &command_buffer_allocate_info, &frame_data.main_command_buffer) != VK_SUCCESS)
        {
            SPDLOG_CRITICAL("Failed to allocate command buffers for image {}", i);
            return false;
        }
    }

    return true;
}

bool RendererVulkan::CreateSyncStructures()
{
    VkFenceCreateInfo fence_info = VulkanInitializers::FenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
    VkSemaphoreCreateInfo semaphore_info = VulkanInitializers::SemaphoreCreateInfo();

    for (uint8_t i = 0; i < m_frame_buffer_amount; i++)
    {
        VulkanFrameData& frame_data = m_frame_data.at(i);
        if (vkCreateFence(m_vulkan_device, &fence_info, nullptr, &frame_data.render_fence) != VK_SUCCESS)
        {
            SPDLOG_CRITICAL("Failed render fence creation for image {}", i);
            return false;
        }

        if (vkCreateSemaphore(m_vulkan_device, &semaphore_info, nullptr, &frame_data.swapchain_semaphore) != VK_SUCCESS)
        {
            SPDLOG_CRITICAL("Failed to create swapchain semaphore for image {}", i);
            return false;
        }

        if (vkCreateSemaphore(m_vulkan_device, &semaphore_info, nullptr, &frame_data.render_semaphore) != VK_SUCCESS)
        {
            SPDLOG_CRITICAL("Failed to create render semaphore for image {}", i);
            return false;
        }
    }

    return true;
}

bool RendererVulkan::DestroySwapchain()
{
    vkDestroySwapchainKHR(m_vulkan_device, m_swapchain, nullptr);
    for (VkImageView swapchain_image_view : m_swapchain_image_views)
    {
        vkDestroyImageView(m_vulkan_device, swapchain_image_view, nullptr);
    }

    return true;
}

bool RendererVulkan::DestroyCommandPoolFenceAndSemaphores()
{
    vkDeviceWaitIdle(m_vulkan_device);
    for (VulkanFrameData& frame_data : m_frame_data)
    {
        vkDestroyCommandPool(m_vulkan_device, frame_data.command_pool, nullptr);
        vkDestroyFence(m_vulkan_device, frame_data.render_fence, nullptr);
        vkDestroySemaphore(m_vulkan_device, frame_data.render_semaphore, nullptr);
        vkDestroySemaphore(m_vulkan_device, frame_data.swapchain_semaphore, nullptr);

        frame_data.deletion_queue.FlushDeletionQueue();
    }

    return true;
}

RendererVulkan::~RendererVulkan()
{
    DestroyCommandPoolFenceAndSemaphores();
    m_global_deletion_queue.FlushDeletionQueue();

    DestroySwapchain();

    vkDestroySurfaceKHR(m_vulkan_instance, m_glfw_window_surface, nullptr);
    vkDestroyDevice(m_vulkan_device, nullptr);

    vkb::destroy_debug_utils_messenger(m_vulkan_instance, m_vulkan_messenger);
    vkDestroyInstance(m_vulkan_instance, nullptr);

    glfwDestroyWindow(m_glfw_window);
    glfwTerminate();
}

VKAPI_ATTR VkBool32 VKAPI_CALL RendererVulkan::VulkanDebugMessenger(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void*)
{
    const std::string message_formatted = std::format("Vulkan - {} - {}", vkb::to_string_message_type(messageType), pCallbackData->pMessage);
    switch (messageSeverity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        SPDLOG_DEBUG(message_formatted);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        SPDLOG_ERROR(message_formatted);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        SPDLOG_WARN(message_formatted);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        SPDLOG_INFO(message_formatted);
        break;
    default:
        SPDLOG_INFO(message_formatted);
        break;
    }

    return VK_FALSE;
}
