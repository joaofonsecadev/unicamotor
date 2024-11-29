#pragma once

#include <cstdint>
#include <vulkan/vulkan_core.h>

namespace VulkanInitializers
{
    VkCommandPoolCreateInfo CommandPoolCreateInfo(const uint8_t queue_family_index, const VkCommandPoolCreateFlags flags = 0);
    VkCommandBufferAllocateInfo CommandBufferAllocateInfo(VkCommandPool command_pool, const uint8_t command_buffer_count = 1);
    VkFenceCreateInfo FenceCreateInfo(VkFenceCreateFlags flags = 0);
    VkSemaphoreCreateInfo SemaphoreCreateInfo(VkSemaphoreCreateFlags flags = 0);
}
