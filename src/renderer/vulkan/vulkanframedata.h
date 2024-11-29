#pragma once

struct VkCommandPool_T;
struct VkCommandBuffer_T;

struct VulkanFrameData
{
    VkCommandPool_T* command_pool;
    VkCommandBuffer_T* main_command_buffer;
};
