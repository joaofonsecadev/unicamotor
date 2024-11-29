#include "vulkaninitializers.h"

#include <tracy/Tracy.hpp>

VkCommandPoolCreateInfo VulkanInitializers::CommandPoolCreateInfo(const uint8_t queue_family_index, const VkCommandPoolCreateFlags flags)
{
    ZoneScoped;

    VkCommandPoolCreateInfo command_pool_create_info = {};
    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.pNext = nullptr;
    command_pool_create_info.queueFamilyIndex = queue_family_index;
    command_pool_create_info.flags = flags;
    return command_pool_create_info;
}

VkCommandBufferAllocateInfo VulkanInitializers::CommandBufferAllocateInfo(VkCommandPool command_pool, const uint8_t command_buffer_count)
{
    ZoneScoped;

    VkCommandBufferAllocateInfo command_buffer_allocate_info = {};
    command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.pNext = nullptr;

    command_buffer_allocate_info.commandPool = command_pool;
    command_buffer_allocate_info.commandBufferCount = command_buffer_count;
    command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY; // @TODO secondary command buffers
    return command_buffer_allocate_info;
}

VkFenceCreateInfo VulkanInitializers::FenceCreateInfo(VkFenceCreateFlags flags)
{
    ZoneScoped;

    VkFenceCreateInfo fence_create_info = { };
    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.pNext = nullptr;
    fence_create_info.flags = flags;
    return fence_create_info;
}

VkSemaphoreCreateInfo VulkanInitializers::SemaphoreCreateInfo(VkSemaphoreCreateFlags flags)
{
    ZoneScoped;

    VkSemaphoreCreateInfo semaphore_create_info = { };
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.pNext = nullptr;
    semaphore_create_info.flags = flags;
    return semaphore_create_info;
}

VkCommandBufferBeginInfo VulkanInitializers::CommandBufferBeginInfo(VkCommandBufferUsageFlags flags)
{
    ZoneScoped;

    VkCommandBufferBeginInfo command_buffer_begin_info = { };
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.pNext = nullptr;
    command_buffer_begin_info.pInheritanceInfo = nullptr;
    command_buffer_begin_info.flags = flags;
    return command_buffer_begin_info;
}

VkImageSubresourceRange VulkanInitializers::ImageSubresourceRange(VkImageAspectFlags flags)
{
    ZoneScoped;

    VkImageSubresourceRange image_subresource_range { };
    image_subresource_range.aspectMask = flags;
    image_subresource_range.baseMipLevel = 0;
    image_subresource_range.levelCount = VK_REMAINING_MIP_LEVELS;
    image_subresource_range.baseArrayLayer = 0;
    image_subresource_range.layerCount = VK_REMAINING_ARRAY_LAYERS;

    return image_subresource_range;
}

VkSemaphoreSubmitInfo VulkanInitializers::SemaphoreSubmitInfo(VkPipelineStageFlags2 pipeline_stage_flags, VkSemaphore semaphore)
{
    ZoneScoped;

    VkSemaphoreSubmitInfo semaphore_submit_info{};
    semaphore_submit_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
    semaphore_submit_info.pNext = nullptr;
    semaphore_submit_info.semaphore = semaphore;
    semaphore_submit_info.stageMask = pipeline_stage_flags;
    semaphore_submit_info.deviceIndex = 0;
    semaphore_submit_info.value = 1;
    return semaphore_submit_info;
}

VkCommandBufferSubmitInfo VulkanInitializers::CommandBufferSubmitInfo(VkCommandBuffer command_buffer)
{
    ZoneScoped;

    VkCommandBufferSubmitInfo command_buffer_submit_info{};
    command_buffer_submit_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
    command_buffer_submit_info.pNext = nullptr;
    command_buffer_submit_info.commandBuffer = command_buffer;
    command_buffer_submit_info.deviceMask = 0;
    return command_buffer_submit_info;
}

VkSubmitInfo2 VulkanInitializers::SubmitInfo(const VkCommandBufferSubmitInfo* command_buffer_submit_info, const VkSemaphoreSubmitInfo* signal_semaphore_submit_info, const VkSemaphoreSubmitInfo* wait_semaphore_submit_info)
{
    ZoneScoped;

    VkSubmitInfo2 submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
    submit_info.pNext = nullptr;

    submit_info.waitSemaphoreInfoCount = wait_semaphore_submit_info == nullptr ? 0 : 1;
    submit_info.pWaitSemaphoreInfos = wait_semaphore_submit_info;

    submit_info.signalSemaphoreInfoCount = signal_semaphore_submit_info == nullptr ? 0 : 1;
    submit_info.pSignalSemaphoreInfos = signal_semaphore_submit_info;

    submit_info.commandBufferInfoCount = 1;
    submit_info.pCommandBufferInfos = command_buffer_submit_info;

    return submit_info;
}

VkImageCreateInfo VulkanInitializers::ImageCreateInfo(VkFormat format, VkImageUsageFlags usage_flags, VkExtent3D extent)
{
    ZoneScoped;

    VkImageCreateInfo image_create_info = {};
    image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.pNext = nullptr;

    image_create_info.imageType = VK_IMAGE_TYPE_2D;

    image_create_info.format = format;
    image_create_info.extent = extent;

    image_create_info.mipLevels = 1;
    image_create_info.arrayLayers = 1;

    // MSAA, not being used thus 1 sample per pixel
    image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;

    // optimal tiling == image is stored in the best gpu format
    image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_create_info.usage = usage_flags;

    return image_create_info;
}

VkImageViewCreateInfo VulkanInitializers::ImageViewCreateInfo(VkFormat format, VkImage image, VkImageAspectFlags aspect_flags)
{
    ZoneScoped;

    // image-view for the depth image for rendering
    VkImageViewCreateInfo image_view_create_info = {};
    image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    image_view_create_info.pNext = nullptr;

    image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    image_view_create_info.image = image;
    image_view_create_info.format = format;
    image_view_create_info.subresourceRange.baseMipLevel = 0;
    image_view_create_info.subresourceRange.levelCount = 1;
    image_view_create_info.subresourceRange.baseArrayLayer = 0;
    image_view_create_info.subresourceRange.layerCount = 1;
    image_view_create_info.subresourceRange.aspectMask = aspect_flags;

    return image_view_create_info;
}
