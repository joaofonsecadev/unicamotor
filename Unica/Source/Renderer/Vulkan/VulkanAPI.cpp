// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "VulkanAPI.h"

#include <algorithm>
#include <map>
#include <set>
#include <vector>

#include "UnicaFileUtilities.h"
#include "fmt/format.h"
#include "shaderc/shaderc.hpp"

#include "UnicaMinimal.h"
#include "VulkanQueueFamilyIndices.h"
#include "Renderer/RenderManager.h"
#include "Shaders/ShaderUtilities.h"

void VulkanAPI::Init()
{
	m_VulkanInstance->Init();
	m_VulkanWindowSurface->Init();
	m_VulkanPhysicalDevice->Init();
	m_VulkanLogicalDevice->Init();
	m_VulkanSwapChain->Init();
	InitVulkanImageViews();
	m_VulkanRenderPass->Init();
	m_VulkanPipeline->Init();
	InitVulkanFramebuffers();
	CreateCommandPool();
}

void VulkanAPI::Tick()
{
	m_SdlRenderWindow->Tick();
}

VulkanQueueFamilyIndices VulkanAPI::GetDeviceQueueFamilies(const VkPhysicalDevice& VulkanPhysicalDevice)
{
	VulkanQueueFamilyIndices QueueFamilyIndices;

	uint32 QueueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(VulkanPhysicalDevice, &QueueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(VulkanPhysicalDevice, &QueueFamilyCount, QueueFamilies.data());

	uint32 QueueFamilyIndex = 0;
	for (const VkQueueFamilyProperties& QueueFamily : QueueFamilies)
	{
		if (QueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			QueueFamilyIndices.SetGraphicsFamily(QueueFamilyIndex);
		}

		VkBool32 PresentImagesSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(VulkanPhysicalDevice, QueueFamilyIndex, m_VulkanWindowSurface->GetVulkanObject(), &PresentImagesSupport);

		if (PresentImagesSupport)
		{
			QueueFamilyIndices.SetPresentImagesFamily(QueueFamilyIndex);
		}

		if (QueueFamilyIndices.WasSet())
		{
			break;
		}
		
		QueueFamilyIndex++;
	}
	
	return QueueFamilyIndices;
}

VulkanSwapChainSupportDetails VulkanAPI::QuerySwapChainSupport(const VkPhysicalDevice& VulkanPhysicalDevice)
{
	VulkanSwapChainSupportDetails SwapChainSupportDetails;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VulkanPhysicalDevice, m_VulkanWindowSurface->GetVulkanObject(), &SwapChainSupportDetails.SurfaceCapabilities);

	uint32 SurfaceFormatsCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanPhysicalDevice, m_VulkanWindowSurface->GetVulkanObject(), &SurfaceFormatsCount, nullptr);
	if (SurfaceFormatsCount > 0)
	{
		SwapChainSupportDetails.SurfaceFormats.resize(SurfaceFormatsCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(VulkanPhysicalDevice, m_VulkanWindowSurface->GetVulkanObject(), &SurfaceFormatsCount, SwapChainSupportDetails.SurfaceFormats.data());
	}

	uint32 PresentModesCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanPhysicalDevice, m_VulkanWindowSurface->GetVulkanObject(), &PresentModesCount, nullptr);
	if (PresentModesCount > 0)
	{
		SwapChainSupportDetails.PresentModes.resize(PresentModesCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(VulkanPhysicalDevice, m_VulkanWindowSurface->GetVulkanObject(), &PresentModesCount, SwapChainSupportDetails.PresentModes.data());
	}
	
	return SwapChainSupportDetails;
}

void VulkanAPI::InitVulkanImageViews()
{
	uint32 SwapChainImageIteration = 0;
	m_VulkanSwapChainImageViews.resize(m_VulkanSwapChain->GetVulkanSwapChainImages().size());
	for (const VkImage& SwapChainImage : m_VulkanSwapChain->GetVulkanSwapChainImages())
	{
		m_VulkanSwapChainImageViews[SwapChainImageIteration] = std::make_unique<VulkanImageView>(this, SwapChainImage);
		m_VulkanSwapChainImageViews[SwapChainImageIteration]->Init();
		SwapChainImageIteration++;
	}
}

void VulkanAPI::InitVulkanFramebuffers()
{
	uint32 SwapChainImageViewLoopIndex = 0;
	m_SwapChainFramebuffers.resize(m_VulkanSwapChainImageViews.size());
	for (const std::unique_ptr<VulkanImageView>& SwapChainImageView : m_VulkanSwapChainImageViews)
	{
		const VkImageView ImageViewAttachments[] = { SwapChainImageView->GetVulkanObject() };
		VkFramebufferCreateInfo FramebufferCreateInfo { };
		FramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		FramebufferCreateInfo.renderPass = m_VulkanRenderPass->GetVulkanObject();
		FramebufferCreateInfo.attachmentCount = 1;
		FramebufferCreateInfo.pAttachments = ImageViewAttachments;
		FramebufferCreateInfo.width = m_VulkanSwapChain->GetVulkanExtent().width;
		FramebufferCreateInfo.height = m_VulkanSwapChain->GetVulkanExtent().height;
		FramebufferCreateInfo.layers = 1;

		if (vkCreateFramebuffer(m_VulkanLogicalDevice->GetVulkanObject(), &FramebufferCreateInfo, nullptr, &m_SwapChainFramebuffers[SwapChainImageViewLoopIndex]) != VK_SUCCESS)
		{
			UNICA_LOG(spdlog::level::critical, "Failed to create a VulkanFramebuffer");
		}
		
		SwapChainImageViewLoopIndex++;
	}
}

void VulkanAPI::CreateCommandPool()
{
	VulkanQueueFamilyIndices QueueFamilyIndices = GetDeviceQueueFamilies(m_VulkanPhysicalDevice->GetVulkanObject());

	VkCommandPoolCreateInfo CommandPoolCreateInfo { };
	CommandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	CommandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	CommandPoolCreateInfo.queueFamilyIndex = QueueFamilyIndices.GetGraphicsFamily().value();

	if (vkCreateCommandPool(m_VulkanLogicalDevice->GetVulkanObject(), &CommandPoolCreateInfo, nullptr, &m_VulkanCommandPool) != VK_SUCCESS)
	{
		UNICA_LOG(spdlog::level::critical, "Failed to create the VulkanCommandPool");
	}
}

void VulkanAPI::Shutdown()
{
	vkDestroyCommandPool(m_VulkanLogicalDevice->GetVulkanObject(), m_VulkanCommandPool, nullptr);
	for (VkFramebuffer_T* SwapChainFramebuffer : m_SwapChainFramebuffers)
	{
		vkDestroyFramebuffer(m_VulkanLogicalDevice->GetVulkanObject(), SwapChainFramebuffer, nullptr);
	}
	
	m_VulkanPipeline->Destroy();
	m_VulkanRenderPass->Destroy();
	
	for (const std::unique_ptr<VulkanImageView>& VulkanImageView : m_VulkanSwapChainImageViews)
	{
		VulkanImageView->Destroy();
	}
	
	m_VulkanSwapChain->Destroy();
	m_VulkanLogicalDevice->Destroy();
	m_VulkanWindowSurface->Destroy();
	m_VulkanInstance->Destroy();
}
