#pragma once

#include <vector>

#include "Renderer/Vulkan/VulkanTypeInterface.h"

class VulkanSwapChain : public VulkanTypeInterface<VkSwapchainKHR>
{
public:
    VulkanSwapChain(VulkanAPI* OwningVulkanAPI) : VulkanTypeInterface(OwningVulkanAPI) { }
    
    void Init() override;
    void Destroy() override;
    
    ~VulkanSwapChain() override = default;

    std::vector<VkImage>& GetVulkanSwapChainImages() { return m_VulkanSwapChainImages; }
    VkFormat& GetVulkanImageFormat() { return m_VulkanSwapChainImageFormat; }
    VkExtent2D& GetVulkanExtent() { return m_VulkanSwapChainExtent; }

private:
    VkSurfaceFormatKHR SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableSurfaceFormats);
    VkPresentModeKHR SelectSwapPresentMode(const std::vector<VkPresentModeKHR>& AvailablePresentModes);
    VkExtent2D SelectSwapExtent(const VkSurfaceCapabilitiesKHR& SurfaceCapabilities);

    VkFormat m_VulkanSwapChainImageFormat;
    VkExtent2D m_VulkanSwapChainExtent;
    
    std::vector<VkImage> m_VulkanSwapChainImages;
};
