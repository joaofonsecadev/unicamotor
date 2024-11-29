#pragma once

#include <vulkan/vulkan_core.h>

namespace VulkanUtilities
{
    void TransitionImage(VkCommandBuffer command_buffer, VkImage image, VkImageLayout image_layout_current, VkImageLayout image_layout_new);
}
