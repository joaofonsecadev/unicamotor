#include "VulkanCommandBuffer.h"

#include "Renderer/Vulkan/VulkanInterface.h"

void VulkanCommandBuffer::Init()
{
    VkCommandBufferAllocateInfo CommandBufferAllocateInfo{};
    CommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    CommandBufferAllocateInfo.commandPool = m_OwningVulkanAPI->GetVulkanCommandPool()->GetVulkanObject();
    CommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    CommandBufferAllocateInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), &CommandBufferAllocateInfo, &m_VulkanObject) != VK_SUCCESS)
    {
        UNICA_LOG_CRITICAL("Failed to allocate VulkanCommandBuffers");
    }
}

void VulkanCommandBuffer::Record(uint32 VulkanImageViewIndex)
{
    VkCommandBufferBeginInfo CommandBufferBeginInfo { };
    CommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(m_VulkanObject, &CommandBufferBeginInfo) != VK_SUCCESS)
    {
        UNICA_LOG_CRITICAL("Failed to begin recording command buffer");
    }

    VkRenderPassBeginInfo RenderPassBeginInfo { };
    RenderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    RenderPassBeginInfo.renderPass = m_OwningVulkanAPI->GetVulkanRenderPass()->GetVulkanObject();
    RenderPassBeginInfo.framebuffer = m_OwningVulkanAPI->GetVulkanFramebuffers().at(VulkanImageViewIndex)->GetVulkanObject();
    RenderPassBeginInfo.renderArea.offset = {0, 0};
    RenderPassBeginInfo.renderArea.extent = m_OwningVulkanAPI->GetVulkanSwapChain()->GetVulkanExtent();

    const VkClearValue ClearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    
    RenderPassBeginInfo.clearValueCount = 1;
    RenderPassBeginInfo.pClearValues = &ClearColor;

    vkCmdBeginRenderPass(m_VulkanObject, &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(m_VulkanObject, VK_PIPELINE_BIND_POINT_GRAPHICS, m_OwningVulkanAPI->GetVulkanPipeline()->GetVulkanObject());

    VkViewport Viewport { };
    Viewport .x = 0.0f;
    Viewport .y = 0.0f;
    Viewport .width = static_cast<float>(m_OwningVulkanAPI->GetVulkanSwapChain()->GetVulkanExtent().width);
    Viewport .height = static_cast<float>(m_OwningVulkanAPI->GetVulkanSwapChain()->GetVulkanExtent().height);
    Viewport .minDepth = 0.0f;
    Viewport .maxDepth = 1.0f;
    vkCmdSetViewport(m_VulkanObject, 0, 1, &Viewport );

    VkRect2D Scissor{};
    Scissor.offset = {0, 0};
    Scissor.extent = m_OwningVulkanAPI->GetVulkanSwapChain()->GetVulkanExtent();
    vkCmdSetScissor(m_VulkanObject, 0, 1, &Scissor);

    vkCmdDraw(m_VulkanObject, 3, 1, 0, 0);
    vkCmdEndRenderPass(m_VulkanObject);
    if (vkEndCommandBuffer(m_VulkanObject) != VK_SUCCESS)
    {
        UNICA_LOG_CRITICAL("Failed to record command buffer!");
    }
}

