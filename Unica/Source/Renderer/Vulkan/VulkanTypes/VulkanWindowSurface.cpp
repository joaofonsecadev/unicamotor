// 2021-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "VulkanWindowSurface.h"

#include "Renderer/Vulkan/VulkanAPI.h"

void VulkanWindowSurface::Init()
{
    if (glfwCreateWindowSurface(m_OwningVulkanAPI->GetVulkanInstance()->GetVulkanObject(), m_OwningVulkanAPI->GetGlfwRenderWindow()->GetGlfwWindow(), nullptr, &m_VulkanObject))
    {
        UNICA_LOG(Fatal, __FUNCTION__, "Failed to create VulkanWindowSurface");
    }
}

void VulkanWindowSurface::Destroy()
{
    vkDestroySurfaceKHR(m_OwningVulkanAPI->GetVulkanInstance()->GetVulkanObject(), m_VulkanObject, nullptr);
}
