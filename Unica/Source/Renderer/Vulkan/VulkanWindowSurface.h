#pragma once

#include "VulkanObjectInterface.h"

class VulkanWindowSurface : public VulkanObjectInterface<VkSurfaceKHR>
{
public:
    VulkanWindowSurface(VulkanAPI* OwningVulkanAPI) : VulkanObjectInterface(OwningVulkanAPI) { }
    
    void Init() override;
    void Destroy() override;

    ~VulkanWindowSurface() override = default;
};
