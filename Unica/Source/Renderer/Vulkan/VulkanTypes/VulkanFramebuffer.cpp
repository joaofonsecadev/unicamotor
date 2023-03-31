#include "VulkanFramebuffer.h"

#include "Log/Logger.h"
#include "Renderer/Vulkan/VulkanInterface.h"

void VulkanFramebuffer::Init()
{
    const VkImageView ImageViewAttachments[] = { m_VulkanImageView->GetVulkanObject() };
    VkFramebufferCreateInfo FramebufferCreateInfo { };
    FramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    FramebufferCreateInfo.renderPass = m_OwningVulkanAPI->GetVulkanRenderPass()->GetVulkanObject();
    FramebufferCreateInfo.attachmentCount = 1;
    FramebufferCreateInfo.pAttachments = ImageViewAttachments;
    FramebufferCreateInfo.width = m_OwningVulkanAPI->GetVulkanSwapChain()->GetVulkanExtent().width;
    FramebufferCreateInfo.height = m_OwningVulkanAPI->GetVulkanSwapChain()->GetVulkanExtent().height;
    FramebufferCreateInfo.layers = 1;

    if (vkCreateFramebuffer(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), &FramebufferCreateInfo, nullptr, &m_VulkanObject) != VK_SUCCESS)
    {
        UNICA_LOG(spdlog::level::critical, "Failed to create a VulkanFramebuffer");
    }
}

void VulkanFramebuffer::Destroy()
{
    vkDestroyFramebuffer(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_VulkanObject, nullptr);
}
