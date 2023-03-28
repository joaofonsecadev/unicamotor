// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include "vulkan/vulkan_core.h"

class VulkanInterface;

template <typename T>
class VulkanTypeInterface
{
public:
    VulkanTypeInterface(VulkanInterface* OwningVulkanAPI) : m_OwningVulkanAPI(OwningVulkanAPI) { }
    virtual void Init() = 0;
    virtual void Destroy() = 0;
    virtual ~VulkanTypeInterface() = default;
    
    virtual T GetVulkanObject() { return m_VulkanObject; }
    
    T m_VulkanObject = VK_NULL_HANDLE;
    VulkanInterface* m_OwningVulkanAPI = nullptr;
};
