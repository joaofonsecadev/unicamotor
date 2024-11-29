#include "vulkaninitializers.h"

VkCommandPoolCreateInfo VulkanInitializers::CommandPoolCreateInfo(const uint8_t queue_family_index, const VkCommandPoolCreateFlags flags)
{
    VkCommandPoolCreateInfo command_pool_create_info = {};
    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.pNext = nullptr;
    command_pool_create_info.queueFamilyIndex = queue_family_index;
    command_pool_create_info.flags = flags;
    return command_pool_create_info;
}

VkCommandBufferAllocateInfo VulkanInitializers::CommandBufferAllocateInfo(VkCommandPool command_pool, const uint8_t command_buffer_count)
{
    VkCommandBufferAllocateInfo command_buffer_allocate_info = {};
    command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.pNext = nullptr;

    command_buffer_allocate_info.commandPool = command_pool;
    command_buffer_allocate_info.commandBufferCount = command_buffer_count;
    command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; // @TODO secondary command buffers
    return command_buffer_allocate_info;
}
