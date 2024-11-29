#pragma once
#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

struct VulkanAllocatedImage
{
    VkImage image;
    VkImageView view;
    VmaAllocation allocation;
    VkExtent3D extent;
    VkFormat format;
};
