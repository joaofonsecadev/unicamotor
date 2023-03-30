#include "VulkanImageView.h"

#include "Renderer/Vulkan/VulkanInterface.h"

void VulkanImageView::Init()
{
    VkImageViewCreateInfo ImageViewCreateInfo { };
    ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    ImageViewCreateInfo.image = m_VulkanSwapChainImage;
    ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    ImageViewCreateInfo.format = m_OwningVulkanAPI->GetVulkanSwapChain()->GetVulkanImageFormat();

    ImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    ImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    ImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    ImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    ImageViewCreateInfo.subresourceRange.levelCount = 1;
    ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    ImageViewCreateInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), &ImageViewCreateInfo, nullptr, &m_VulkanObject) != VK_SUCCESS)
    {
        UNICA_LOG(spdlog::level::critical, "Failed to create VulkanImageViews");
    }
}

void VulkanImageView::Destroy()
{
    vkDestroyImageView(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_VulkanObject, nullptr);
}
