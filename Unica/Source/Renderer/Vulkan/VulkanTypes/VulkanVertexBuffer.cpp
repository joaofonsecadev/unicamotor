#include "VulkanVertexBuffer.h"

#include "Renderer/Vulkan/VulkanInterface.h"
#include "Renderer/Vulkan/VulkanVertex.h"

void VulkanVertexBuffer::Init()
{
    const std::vector<VulkanVertex>& Vertices = m_OwningVulkanAPI->GetHardcodedVertices();
    
    VkBufferCreateInfo VertexBufferCreateInfo{};
    VertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    VertexBufferCreateInfo.size = sizeof(Vertices[0]) * Vertices.size();
    VertexBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    VertexBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), &VertexBufferCreateInfo, nullptr, &m_VulkanObject) != VK_SUCCESS)
    {
        UNICA_LOG_CRITICAL("Failed to create vertex buffer!");
    }

    VkMemoryRequirements VulkanMemoryRequirements;
    vkGetBufferMemoryRequirements(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_VulkanObject, &VulkanMemoryRequirements);

    VkMemoryAllocateInfo VulkanMemoryAllocInfo{};
    VulkanMemoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    VulkanMemoryAllocInfo.allocationSize = VulkanMemoryRequirements.size;
    VulkanMemoryAllocInfo.memoryTypeIndex = FindGpuMemoryType(VulkanMemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), &VulkanMemoryAllocInfo, nullptr, &m_VulkanDeviceMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_VulkanObject, m_VulkanDeviceMemory, 0);

    void* data;
    vkMapMemory(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_VulkanDeviceMemory, 0, VertexBufferCreateInfo.size, 0, &data);
    memcpy(data, Vertices.data(), (size_t) VertexBufferCreateInfo.size);
    vkUnmapMemory(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_VulkanDeviceMemory);
}

uint32 VulkanVertexBuffer::FindGpuMemoryType(uint32 TypeFilter, VkMemoryPropertyFlags PropertyFlags) const
{
    VkPhysicalDeviceMemoryProperties GpuMemoryProperties;
    vkGetPhysicalDeviceMemoryProperties(m_OwningVulkanAPI->GetVulkanPhysicalDevice()->GetVulkanObject(), &GpuMemoryProperties);

    for (uint32 i = 0; i < GpuMemoryProperties.memoryTypeCount; i++)
    {
        if ((TypeFilter & (1 << i)) && (GpuMemoryProperties.memoryTypes[i].propertyFlags & PropertyFlags) == PropertyFlags)
        {
            return i;
        }
    }

    UNICA_LOG_CRITICAL("Failed to find suitable memory type!");
}

void VulkanVertexBuffer::Destroy()
{
    vkDestroyBuffer(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_VulkanObject, nullptr);
}
