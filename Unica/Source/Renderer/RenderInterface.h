// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <vector>
#include <vulkan/vulkan_core.h>

class RenderInterface
{
public:
    RenderInterface();
    ~RenderInterface();

private:
    void CreateVulkanInstance();
    void AddRequiredExtensions(VkInstanceCreateInfo& VulkanCreateInfo, std::vector<const char*>& RequiredExtensions);
    void AddValidationLayers(VkInstanceCreateInfo& VulkanCreateInfo);

    VkInstance m_VulkanInstance;
};
