#include "VulkanCommandBuffer.h"

#include "Log/Logger.h"
#include "Renderer/Vulkan/VulkanInterface.h"

void VulkanCommandBuffer::Init()
{
    m_VulkanCommandBuffers.resize(m_OwningVulkanAPI->GetMaxFramesInFlight());
    
    VkCommandBufferAllocateInfo CommandBufferAllocateInfo{};
    CommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    CommandBufferAllocateInfo.commandPool = m_OwningVulkanAPI->GetVulkanCommandPool()->GetVulkanObject();
    CommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    CommandBufferAllocateInfo.commandBufferCount = static_cast<uint32>(m_VulkanCommandBuffers.size());

    if (vkAllocateCommandBuffers(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), &CommandBufferAllocateInfo, m_VulkanCommandBuffers.data()) != VK_SUCCESS)
    {
        UNICA_LOG_CRITICAL("Failed to allocate VulkanCommandBuffers");
    }

    UNICA_LOG_TRACE("VulkanCommandBuffer created");
}

void VulkanCommandBuffer::Record(uint8 VulkanCommandBufferIndex, uint32 VulkanImageIndex)
{
    UNICA_PROFILE_FUNCTION
    
    VkCommandBufferBeginInfo CommandBufferBeginInfo { };
    CommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(m_VulkanCommandBuffers[VulkanCommandBufferIndex], &CommandBufferBeginInfo) != VK_SUCCESS)
    {
        UNICA_LOG_CRITICAL("Failed to begin recording command buffer");
    }

    VkRenderPassBeginInfo RenderPassBeginInfo { };
    RenderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    RenderPassBeginInfo.renderPass = m_OwningVulkanAPI->GetVulkanRenderPass()->GetVulkanObject();
    RenderPassBeginInfo.framebuffer = m_OwningVulkanAPI->GetVulkanFramebuffers().at(VulkanImageIndex)->GetVulkanObject();
    RenderPassBeginInfo.renderArea.offset = {0, 0};
    RenderPassBeginInfo.renderArea.extent = m_OwningVulkanAPI->GetVulkanSwapChain()->GetVulkanExtent();

    constexpr VkClearValue ClearColor = {{{0.01f, 0.01f, 0.01f, 1.0f}}};
    
    RenderPassBeginInfo.clearValueCount = 1;
    RenderPassBeginInfo.pClearValues = &ClearColor;

    vkCmdBeginRenderPass(m_VulkanCommandBuffers[VulkanCommandBufferIndex], &RenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(m_VulkanCommandBuffers[VulkanCommandBufferIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, m_OwningVulkanAPI->GetVulkanPipeline()->GetVulkanObject());

    VkViewport Viewport { };
    Viewport .x = 0.0f;
    Viewport .y = 0.0f;
    Viewport .width = static_cast<float>(m_OwningVulkanAPI->GetVulkanSwapChain()->GetVulkanExtent().width);
    Viewport .height = static_cast<float>(m_OwningVulkanAPI->GetVulkanSwapChain()->GetVulkanExtent().height);
    Viewport .minDepth = 0.0f;
    Viewport .maxDepth = 1.0f;
    vkCmdSetViewport(m_VulkanCommandBuffers[VulkanCommandBufferIndex], 0, 1, &Viewport );

    VkRect2D Scissor{};
    Scissor.offset = {0, 0};
    Scissor.extent = m_OwningVulkanAPI->GetVulkanSwapChain()->GetVulkanExtent();
    vkCmdSetScissor(m_VulkanCommandBuffers[VulkanCommandBufferIndex], 0, 1, &Scissor);

    vkCmdDraw(m_VulkanCommandBuffers[VulkanCommandBufferIndex], 3, 1, 0, 0);
    vkCmdEndRenderPass(m_VulkanCommandBuffers[VulkanCommandBufferIndex]);
    if (vkEndCommandBuffer(m_VulkanCommandBuffers[VulkanCommandBufferIndex]) != VK_SUCCESS)
    {
        UNICA_LOG_CRITICAL("Failed to record command buffer!");
    }
}

