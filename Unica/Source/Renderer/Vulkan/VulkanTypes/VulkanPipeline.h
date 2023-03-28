#pragma once
#include <vector>

#include "Renderer/Vulkan/VulkanTypeInterface.h"

class VulkanPipeline : public VulkanTypeInterface<VkPipeline>
{
public:
    VulkanPipeline(VulkanAPI* OwningVulkanAPI) : VulkanTypeInterface(OwningVulkanAPI) { }
    
    void Init() override;
    void Destroy() override;
    
    ~VulkanPipeline() override = default;

private:
    VkShaderModule CreateShaderModule(const std::vector<char>& ShaderBinary);
    
    VkPipelineLayout m_VulkanPipelineLayout = VK_NULL_HANDLE;
};
