#pragma once

#include <cstdint>
#include <vulkan/vulkan_core.h>

namespace VulkanInitializers
{
    VkCommandPoolCreateInfo CommandPoolCreateInfo(const uint8_t queue_family_index, const VkCommandPoolCreateFlags flags = 0);
    VkCommandBufferAllocateInfo CommandBufferAllocateInfo(VkCommandPool command_pool, const uint8_t command_buffer_count = 1);
    VkFenceCreateInfo FenceCreateInfo(VkFenceCreateFlags flags = 0);
    VkSemaphoreCreateInfo SemaphoreCreateInfo(VkSemaphoreCreateFlags flags = 0);
    VkCommandBufferBeginInfo CommandBufferBeginInfo(VkCommandBufferUsageFlags flags = 0);
    VkImageSubresourceRange ImageSubresourceRange(VkImageAspectFlags flags = 0);
    VkSemaphoreSubmitInfo SemaphoreSubmitInfo(VkPipelineStageFlags2 pipeline_stage_flags, VkSemaphore semaphore);
    VkCommandBufferSubmitInfo CommandBufferSubmitInfo(VkCommandBuffer command_buffer);
    VkSubmitInfo2 SubmitInfo(const VkCommandBufferSubmitInfo* command_buffer_submit_info, const VkSemaphoreSubmitInfo* signal_semaphore_submit_info, const VkSemaphoreSubmitInfo* wait_semaphore_submit_info);
}
