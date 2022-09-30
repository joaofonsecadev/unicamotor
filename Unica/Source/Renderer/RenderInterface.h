// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <vulkan/vulkan_core.h>

class RenderInterface
{
public:
    RenderInterface();
    ~RenderInterface();

private:
    void CreateVulkanInstance();
    bool CheckAvailableValidationLayers();
    void LogVulkanInstanceExtensions();

    VkInstance m_VulkanInstance;
};
