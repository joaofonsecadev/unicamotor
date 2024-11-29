#pragma once

#include "vulkandeletionqueue.h"

struct VkCommandPool_T;
struct VkCommandBuffer_T;
struct VkSemaphore_T;
struct VkFence_T;

struct VulkanFrameData
{
    VkCommandPool_T* command_pool = nullptr;
    VkCommandBuffer_T* main_command_buffer = nullptr;
    VkSemaphore_T* swapchain_semaphore = nullptr;
    VkSemaphore_T* render_semaphore = nullptr;
    VkFence_T* render_fence = nullptr;

    VulkanDeletionQueue deletion_queue;
};
