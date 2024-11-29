#pragma once

#include <vulkan/vulkan_core.h>

namespace VulkanUtilities
{
    void TransitionImage(VkCommandBuffer command_buffer, VkImage image, VkImageLayout image_layout_current, VkImageLayout image_layout_new);
    void CopyImageToImage(VkCommandBuffer command_buffer, VkImage source_image, VkImage dest_image, VkExtent2D src_size, VkExtent2D dest_size);
}
