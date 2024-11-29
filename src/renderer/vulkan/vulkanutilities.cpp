#include "vulkanutilities.h"

#include <tracy/Tracy.hpp>

#include "vulkaninitializers.h"

void VulkanUtilities::TransitionImage(VkCommandBuffer command_buffer, VkImage image, VkImageLayout image_layout_current, VkImageLayout image_layout_new)
{
    ZoneScoped;

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

void VulkanUtilities::CopyImageToImage(VkCommandBuffer command_buffer, VkImage source_image, VkImage dest_image, VkExtent2D src_size, VkExtent2D dest_size)
{
    ZoneScoped;

    VkImageBlit2 image_blit_region { .sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2, .pNext = nullptr };

    image_blit_region.srcOffsets[1].x = src_size.width;
    image_blit_region.srcOffsets[1].y = src_size.height;
    image_blit_region.srcOffsets[1].z = 1;

    image_blit_region.dstOffsets[1].x = dest_size.width;
    image_blit_region.dstOffsets[1].y = dest_size.height;
    image_blit_region.dstOffsets[1].z = 1;

    image_blit_region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    image_blit_region.srcSubresource.baseArrayLayer = 0;
    image_blit_region.srcSubresource.layerCount = 1;
    image_blit_region.srcSubresource.mipLevel = 0;

    image_blit_region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    image_blit_region.dstSubresource.baseArrayLayer = 0;
    image_blit_region.dstSubresource.layerCount = 1;
    image_blit_region.dstSubresource.mipLevel = 0;

    VkBlitImageInfo2 blit_image_info { .sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2, .pNext = nullptr };
    blit_image_info.dstImage = dest_image;
    blit_image_info.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    blit_image_info.srcImage = source_image;
    blit_image_info.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    blit_image_info.filter = VK_FILTER_LINEAR;
    blit_image_info.regionCount = 1;
    blit_image_info.pRegions = &image_blit_region;

    vkCmdBlitImage2(command_buffer, &blit_image_info);
}
