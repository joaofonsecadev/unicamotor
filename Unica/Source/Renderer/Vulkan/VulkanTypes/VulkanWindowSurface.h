// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include "Renderer/Vulkan/VulkanTypeInterface.h"

class VulkanWindowSurface : public VulkanTypeInterface<VkSurfaceKHR>
{
public:
    VulkanWindowSurface(VulkanAPI* OwningVulkanAPI) : VulkanTypeInterface(OwningVulkanAPI) { }
    
    void Init() override;
    void Destroy() override;

    ~VulkanWindowSurface() override = default;
};
