#include "vulkanutilities.h"

#include "vulkaninitializers.h"

void VulkanUtilities::TransitionImage(VkCommandBuffer command_buffer, VkImage image, VkImageLayout image_layout_current, VkImageLayout image_layout_new)
{
    VkImageMemoryBarrier2 image_memory_barrier {.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2};
    image_memory_barrier.pNext = nullptr;

    //@TODO Using ALL_COMMANDS is inefficient, if many transitions per frame => StageMasks accurate to the work being done
    //https://github.com/KhronosGroup/Vulkan-Docs/wiki/Synchronization-Examples

    image_memory_barrier.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
    image_memory_barrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
    image_memory_barrier.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
    image_memory_barrier.dstAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT;

    image_memory_barrier.oldLayout = image_layout_current;
    image_memory_barrier.newLayout = image_layout_new;


    VkImageAspectFlags image_aspect_flags = image_layout_new == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
    image_memory_barrier.subresourceRange = VulkanInitializers::ImageSubresourceRange(image_aspect_flags);
    image_memory_barrier.image = image;

    VkDependencyInfo dependency_info {};
    dependency_info.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependency_info.pNext = nullptr;

    dependency_info.imageMemoryBarrierCount = 1;
    dependency_info.pImageMemoryBarriers = &image_memory_barrier;

    vkCmdPipelineBarrier2(command_buffer, &dependency_info);
}
