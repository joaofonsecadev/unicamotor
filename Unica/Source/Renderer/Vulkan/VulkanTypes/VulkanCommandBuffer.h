#pragma once

#include "Renderer/Vulkan/VulkanTypeInterface.h"
#include "UnicaMinimal.h"

class VulkanCommandBuffer : public VulkanTypeInterface<VkCommandBuffer> 
{
public:
    VulkanCommandBuffer(VulkanInterface* OwningVulkanAPI) : VulkanTypeInterface(OwningVulkanAPI) { }
    ~VulkanCommandBuffer() override = default;

    void Init() override;
    void Destroy() override { }
        
    void Record(uint32 VulkanImageIndex);

    VkCommandBuffer* GetCommandBufferObject() { return &m_VulkanObject; }
};
