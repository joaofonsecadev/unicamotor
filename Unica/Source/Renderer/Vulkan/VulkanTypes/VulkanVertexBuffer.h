#pragma once
#include "UnicaMinimal.h"
#include "Renderer/Vulkan/VulkanTypeInterface.h"

class VulkanVertexBuffer : public VulkanTypeInterface<VkBuffer>
{
public:
    VulkanVertexBuffer(VulkanInterface* OwningVulkanAPI) : VulkanTypeInterface(OwningVulkanAPI) { }
    
    void Init() override;
    void Destroy() override;

    VkDeviceMemory GetVulkanDeviceMemory() const { return m_VulkanDeviceMemory; }
    
    ~VulkanVertexBuffer() override = default;

private:
    uint32 FindGpuMemoryType(uint32 TypeFilter, VkMemoryPropertyFlags PropertyFlags) const;

    VkDeviceMemory m_VulkanDeviceMemory;
};
