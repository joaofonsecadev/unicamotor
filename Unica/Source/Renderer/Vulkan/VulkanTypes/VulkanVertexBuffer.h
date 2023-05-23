#pragma once
#include "UnicaMinimal.h"
#include "Renderer/Vulkan/VulkanTypeInterface.h"

class VulkanVertexBuffer : public VulkanTypeInterface<VkBuffer>
{
public:
    VulkanVertexBuffer(VulkanInterface* OwningVulkanAPI) : VulkanTypeInterface(OwningVulkanAPI) { }
    
    void Init() override;
    void Destroy() override;

    VkDeviceMemory GetVulkanDeviceMemory() const { return m_VertexBufferMemory; }

    VkBuffer GetIndexBuffer() const { return m_IndexBuffer; }
    
    ~VulkanVertexBuffer() override = default;

private:
    void CreateBuffer(uint64 Size, VkBufferUsageFlags UsageFlags, VkMemoryPropertyFlags PropertyFlags, VkBuffer& OutBuffer, VkDeviceMemory& OutBufferMemory);
    void CopyBuffer(VkBuffer SourceBuffer, VkBuffer DestinationBuffer, VkDeviceSize Size);
    
    uint32 FindGpuMemoryType(uint32 TypeFilter, VkMemoryPropertyFlags PropertyFlags) const;
    
    VkDeviceMemory m_VertexBufferMemory;

    VkBuffer m_IndexBuffer;
    VkDeviceMemory m_IndexBufferMemory;
};
