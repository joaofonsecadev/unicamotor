#include "VulkanRenderPass.h"

#include "Log/Logger.h"
#include "Renderer/Vulkan/VulkanInterface.h"

void VulkanRenderPass::Init()
{
    VkAttachmentDescription VulkanColorAttachment { };
    VulkanColorAttachment.format = m_OwningVulkanAPI->GetVulkanSwapChain()->GetVulkanImageFormat();
    VulkanColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    VulkanColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    VulkanColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    VulkanColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    VulkanColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    VulkanColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference VulkanColorAttachmentRef { };
    VulkanColorAttachmentRef.attachment = 0;
    VulkanColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription VulkanSubpass { };
    VulkanSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    VulkanSubpass.colorAttachmentCount = 1;
    VulkanSubpass.pColorAttachments = &VulkanColorAttachmentRef;

    VkSubpassDependency VulkanSubpassDependency{};
    VulkanSubpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    VulkanSubpassDependency.dstSubpass = 0;
    VulkanSubpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VulkanSubpassDependency.srcAccessMask = 0;
    VulkanSubpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VulkanSubpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo VulkanRenderPassCreateInfo { };
    VulkanRenderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    VulkanRenderPassCreateInfo.attachmentCount = 1;
    VulkanRenderPassCreateInfo.pAttachments = &VulkanColorAttachment;
    VulkanRenderPassCreateInfo.subpassCount = 1;
    VulkanRenderPassCreateInfo.pSubpasses = &VulkanSubpass;
    VulkanRenderPassCreateInfo.dependencyCount = 1;
    VulkanRenderPassCreateInfo.pDependencies = &VulkanSubpassDependency;    

    if (vkCreateRenderPass(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), &VulkanRenderPassCreateInfo, nullptr, &m_VulkanObject) != VK_SUCCESS)
    {
        UNICA_LOG(spdlog::level::critical, "Failed to create VulkanRenderPass");
    }
}

void VulkanRenderPass::Destroy()
{
    vkDestroyRenderPass(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_VulkanObject, nullptr);
}
