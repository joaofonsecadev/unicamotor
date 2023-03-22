// 2021-2023 Copyright joaofonseca.dev, All Rights Reserved.

﻿#pragma once

#include "vulkan/vulkan_core.h"

class VulkanAPI;

template <typename T>
class VulkanTypeInterface
{
public:
    VulkanTypeInterface(VulkanAPI* OwningVulkanAPI) : m_OwningVulkanAPI(OwningVulkanAPI) { }
    virtual void Init() = 0;
    virtual void Destroy() = 0;
    virtual ~VulkanTypeInterface() = default;
    
    virtual T GetVulkanObject() { return m_VulkanObject; }
    
    T m_VulkanObject = VK_NULL_HANDLE;
    VulkanAPI* m_OwningVulkanAPI = nullptr;
};
