#include "VulkanCommandPool.h"

#include "Renderer/Vulkan/VulkanInterface.h"
#include "Renderer/Vulkan/VulkanQueueFamilyIndices.h"

void VulkanCommandPool::Init()
{
    VulkanQueueFamilyIndices QueueFamilyIndices = m_OwningVulkanAPI->GetDeviceQueueFamilies(m_OwningVulkanAPI->GetVulkanPhysicalDevice()->GetVulkanObject());

    VkCommandPoolCreateInfo CommandPoolCreateInfo { };
    CommandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    CommandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    CommandPoolCreateInfo.queueFamilyIndex = QueueFamilyIndices.GetGraphicsFamily().value();

    if (vkCreateCommandPool(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), &CommandPoolCreateInfo, nullptr, &m_VulkanObject) != VK_SUCCESS)
    {
        UNICA_LOG(spdlog::level::critical, "Failed to create the VulkanCommandPool");
    }
}

void VulkanCommandPool::Destroy()
{
    vkDestroyCommandPool(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_VulkanObject, nullptr);
}
