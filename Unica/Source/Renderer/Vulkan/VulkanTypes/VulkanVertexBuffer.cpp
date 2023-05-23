#include "VulkanVertexBuffer.h"

#include "Renderer/Vulkan/VulkanInterface.h"
#include "Renderer/Vulkan/VulkanVertex.h"

void VulkanVertexBuffer::Init()
{
    const std::vector<VulkanVertex>& Vertices = m_OwningVulkanAPI->GetHardcodedVertices();
    const std::vector<uint16>& Indices = m_OwningVulkanAPI->GetHardcodedIndices();
    const VkDeviceSize BufferSize = sizeof(Vertices[0]) * Vertices.size();

    // Vertex staging buffer
    VkBuffer StagingBuffer;
    VkDeviceMemory StagingBufferMemory;
    CreateBuffer(BufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, StagingBuffer, StagingBufferMemory);

    void* data;
    vkMapMemory(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), StagingBufferMemory, 0, BufferSize, 0, &data);
    memcpy(data, Vertices.data(), (size_t) BufferSize);
    vkUnmapMemory(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), StagingBufferMemory);

    // Vertex buffer
    CreateBuffer(BufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VulkanObject, m_VertexBufferMemory);

    CopyBuffer(StagingBuffer, m_VulkanObject, BufferSize);
    vkDestroyBuffer(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), StagingBuffer, nullptr);
    vkFreeMemory(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), StagingBufferMemory, nullptr);

    // Indices staging buffer
    VkBuffer IndexStagingBuffer;
    VkDeviceMemory IndexStagingBufferMemory;
    CreateBuffer(BufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, IndexStagingBuffer, IndexStagingBufferMemory);

    void* IndexData;
    vkMapMemory(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), IndexStagingBufferMemory, 0, BufferSize, 0, &IndexData);
    memcpy(IndexData, Indices.data(), (size_t) BufferSize);
    vkUnmapMemory(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), IndexStagingBufferMemory);

    // Indices buffer
    CreateBuffer(BufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_IndexBuffer, m_IndexBufferMemory);

    CopyBuffer(IndexStagingBuffer, m_IndexBuffer, BufferSize);
    vkDestroyBuffer(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), IndexStagingBuffer, nullptr);
    vkFreeMemory(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), IndexStagingBufferMemory, nullptr);
}

void VulkanVertexBuffer::CreateBuffer(uint64 Size, VkBufferUsageFlags UsageFlags, VkMemoryPropertyFlags PropertyFlags, VkBuffer& OutBuffer, VkDeviceMemory& OutBufferMemory)
{
    VkBufferCreateInfo VertexBufferCreateInfo{};
    VertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    VertexBufferCreateInfo.size = Size;
    VertexBufferCreateInfo.usage = UsageFlags;
    VertexBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), &VertexBufferCreateInfo, nullptr, &OutBuffer) != VK_SUCCESS)
    {
        UNICA_LOG_CRITICAL("Failed to create buffer!");
    }

    VkMemoryRequirements VulkanMemoryRequirements;
    vkGetBufferMemoryRequirements(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), OutBuffer, &VulkanMemoryRequirements);

    VkMemoryAllocateInfo VulkanMemoryAllocInfo{};
    VulkanMemoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    VulkanMemoryAllocInfo.allocationSize = VulkanMemoryRequirements.size;
    VulkanMemoryAllocInfo.memoryTypeIndex = FindGpuMemoryType(VulkanMemoryRequirements.memoryTypeBits, PropertyFlags);

    if (vkAllocateMemory(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), &VulkanMemoryAllocInfo, nullptr, &OutBufferMemory) != VK_SUCCESS) {
        UNICA_LOG_CRITICAL("Failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), OutBuffer, OutBufferMemory, 0);
}

void VulkanVertexBuffer::CopyBuffer(VkBuffer SourceBuffer, VkBuffer DestinationBuffer, VkDeviceSize Size)
{
    VkCommandBufferAllocateInfo CommandBufferAllocateInfo{};
    CommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    CommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    CommandBufferAllocateInfo.commandPool = m_OwningVulkanAPI->GetVulkanCommandPool()->GetVulkanObject();
    CommandBufferAllocateInfo.commandBufferCount = 1;

    VkCommandBuffer CopyCommandBuffer;
    vkAllocateCommandBuffers(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), &CommandBufferAllocateInfo, &CopyCommandBuffer);

    VkCommandBufferBeginInfo CopyCommandBufferBeginInfo{};
    CopyCommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    CopyCommandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(CopyCommandBuffer, &CopyCommandBufferBeginInfo);

    VkBufferCopy BufferCopyRegion{};
    BufferCopyRegion.srcOffset = 0; // Optional
    BufferCopyRegion.dstOffset = 0; // Optional
    BufferCopyRegion.size = Size;
    vkCmdCopyBuffer(CopyCommandBuffer, SourceBuffer, DestinationBuffer, 1, &BufferCopyRegion);

    vkEndCommandBuffer(CopyCommandBuffer);

    VkSubmitInfo SubmitInfo{};
    SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    SubmitInfo.commandBufferCount = 1;
    SubmitInfo.pCommandBuffers = &CopyCommandBuffer;

    vkQueueSubmit(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanGraphicsQueue(), 1, &SubmitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanGraphicsQueue());

    vkFreeCommandBuffers(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_OwningVulkanAPI->GetVulkanCommandPool()->GetVulkanObject(), 1, &CopyCommandBuffer);
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
    vkDestroyBuffer(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_IndexBuffer, nullptr);
    vkFreeMemory(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_IndexBufferMemory, nullptr);
    
    vkDestroyBuffer(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_VulkanObject, nullptr);
    vkFreeMemory(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_VertexBufferMemory, nullptr);
}
