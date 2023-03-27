#include "VulkanSwapChain.h"

#include "Renderer/Vulkan/VulkanAPI.h"
#include "Renderer/Vulkan/VulkanSwapChainSupportDetails.h"
#include "Renderer/Vulkan/VulkanQueueFamilyIndices.h"

void VulkanSwapChain::Init()
{
    const VulkanSwapChainSupportDetails SwapChainSupportDetails = m_OwningVulkanAPI->QuerySwapChainSupport(m_OwningVulkanAPI->GetVulkanPhysicalDevice()->GetVulkanObject());
	const VkSurfaceFormatKHR SurfaceFormat = SelectSwapSurfaceFormat(SwapChainSupportDetails.SurfaceFormats);
	const VkPresentModeKHR PresentMode = SelectSwapPresentMode(SwapChainSupportDetails.PresentModes);
	const VkExtent2D Extent = SelectSwapExtent(SwapChainSupportDetails.SurfaceCapabilities);

	uint32 SwapImageCount = SwapChainSupportDetails.SurfaceCapabilities.minImageCount + 1;
	if (SwapChainSupportDetails.SurfaceCapabilities.maxImageCount > 0 && SwapImageCount > SwapChainSupportDetails.SurfaceCapabilities.maxImageCount)
	{
		SwapImageCount = SwapChainSupportDetails.SurfaceCapabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR SwapChainCreateInfo { };
	SwapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	SwapChainCreateInfo.surface = m_OwningVulkanAPI->GetVulkanWindowSurface()->GetVulkanObject();
	SwapChainCreateInfo.minImageCount = SwapImageCount;
	SwapChainCreateInfo.imageFormat = SurfaceFormat.format;
	SwapChainCreateInfo.imageColorSpace = SurfaceFormat.colorSpace;
	SwapChainCreateInfo.imageExtent = Extent;
	SwapChainCreateInfo.imageArrayLayers = 1;
	SwapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	VulkanQueueFamilyIndices QueueFamilyIndices = m_OwningVulkanAPI->GetDeviceQueueFamilies(m_OwningVulkanAPI->GetVulkanPhysicalDevice()->GetVulkanObject());
	const uint32 QueueFamilyIndicesArray[] = { QueueFamilyIndices.GetGraphicsFamily().value(), QueueFamilyIndices.GetPresentImagesFamily().value() };

	if (QueueFamilyIndices.GetGraphicsFamily() != QueueFamilyIndices.GetPresentImagesFamily())
	{
		SwapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		SwapChainCreateInfo.queueFamilyIndexCount = 2;
		SwapChainCreateInfo.pQueueFamilyIndices = QueueFamilyIndicesArray;
	}
	else
	{
		SwapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		SwapChainCreateInfo.queueFamilyIndexCount = 0;
		SwapChainCreateInfo.pQueueFamilyIndices = nullptr;
	}

	SwapChainCreateInfo.preTransform = SwapChainSupportDetails.SurfaceCapabilities.currentTransform;
	SwapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	SwapChainCreateInfo.presentMode = PresentMode;
	SwapChainCreateInfo.clipped = VK_TRUE;
	SwapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), &SwapChainCreateInfo, nullptr, &m_VulkanObject) != VK_SUCCESS)
	{
		UNICA_LOG(spdlog::level::critical, "Failed to create VulkanSwapChain");
		return;
	}

	vkGetSwapchainImagesKHR(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_VulkanObject, &SwapImageCount, nullptr);
	m_VulkanSwapChainImages.resize(SwapImageCount);
	vkGetSwapchainImagesKHR(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_VulkanObject, &SwapImageCount, m_VulkanSwapChainImages.data());

	m_VulkanSwapChainImageFormat = SurfaceFormat.format;
	m_VulkanSwapChainExtent = Extent;
}

VkSurfaceFormatKHR VulkanSwapChain::SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableSurfaceFormats)
{
	for (const VkSurfaceFormatKHR& SurfaceFormat : AvailableSurfaceFormats)
	{
		if (SurfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB && SurfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return SurfaceFormat;
		}
	}

	// TODO: Rank surface formats to default to the best possible one
	return AvailableSurfaceFormats[0];
}

VkPresentModeKHR VulkanSwapChain::SelectSwapPresentMode(const std::vector<VkPresentModeKHR>& AvailablePresentModes)
{
	for (const VkPresentModeKHR& PresentMode : AvailablePresentModes)
	{
		if (PresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return PresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanSwapChain::SelectSwapExtent(const VkSurfaceCapabilitiesKHR& SurfaceCapabilities)
{
	if (SurfaceCapabilities.currentExtent.width != std::numeric_limits<uint32>::max())
	{
		return SurfaceCapabilities.currentExtent;
	}

	int32 Width, Height;
	SDL_GetWindowSizeInPixels(m_OwningVulkanAPI->GetSdlRenderWindow()->GetSdlWindow(), &Width, &Height);

	VkExtent2D VulkanExtent = { static_cast<uint32>(Width), static_cast<uint32>(Height) };
	VulkanExtent.width = std::clamp(VulkanExtent.width, SurfaceCapabilities.minImageExtent.width, SurfaceCapabilities.maxImageExtent.width);
	VulkanExtent.height = std::clamp(VulkanExtent.height, SurfaceCapabilities.minImageExtent.height, SurfaceCapabilities.maxImageExtent.height);

	return VulkanExtent;
}

void VulkanSwapChain::Destroy()
{
	vkDestroySwapchainKHR(m_OwningVulkanAPI->GetVulkanLogicalDevice()->GetVulkanObject(), m_VulkanObject, nullptr);
}
