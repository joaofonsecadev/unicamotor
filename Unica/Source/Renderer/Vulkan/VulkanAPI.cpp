// 2021-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "VulkanAPI.h"

#include <algorithm>
#include <map>
#include <set>
#include <vector>

#include "UnicaFileUtilities.h"
#include "GLFW/glfw3.h"
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
	CreateVulkanLogicalDevice();
	CreateSwapChain();
	CreateImageViews();
	CreateRenderPass();
	CreateGraphicsPipeline();
	CreateFramebuffers();
	CreateCommandPool();
}

void VulkanAPI::Tick()
{
	m_GlfwRenderWindow->Tick();
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

VkSurfaceFormatKHR VulkanAPI::SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableSurfaceFormats)
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

VkPresentModeKHR VulkanAPI::SelectSwapPresentMode(const std::vector<VkPresentModeKHR>& AvailablePresentModes)
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

VkExtent2D VulkanAPI::SelectSwapExtent(const VkSurfaceCapabilitiesKHR& SurfaceCapabilities)
{
	if (SurfaceCapabilities.currentExtent.width != std::numeric_limits<uint32>::max())
	{
		return SurfaceCapabilities.currentExtent;
	}

	int32 Width, Height;
	glfwGetFramebufferSize(m_GlfwRenderWindow->GetGlfwWindow(), &Width, &Height);

	VkExtent2D VulkanExtent = { static_cast<uint32>(Width), static_cast<uint32>(Height) };
	VulkanExtent.width = std::clamp(VulkanExtent.width, SurfaceCapabilities.minImageExtent.width, SurfaceCapabilities.maxImageExtent.width);
	VulkanExtent.height = std::clamp(VulkanExtent.height, SurfaceCapabilities.minImageExtent.height, SurfaceCapabilities.maxImageExtent.height);

	return VulkanExtent;
}

void VulkanAPI::CreateSwapChain()
{
	const VulkanSwapChainSupportDetails SwapChainSupportDetails = QuerySwapChainSupport(m_VulkanPhysicalDevice->GetVulkanObject());
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
	SwapChainCreateInfo.surface = m_VulkanWindowSurface->GetVulkanObject();
	SwapChainCreateInfo.minImageCount = SwapImageCount;
	SwapChainCreateInfo.imageFormat = SurfaceFormat.format;
	SwapChainCreateInfo.imageColorSpace = SurfaceFormat.colorSpace;
	SwapChainCreateInfo.imageExtent = Extent;
	SwapChainCreateInfo.imageArrayLayers = 1;
	SwapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	VulkanQueueFamilyIndices QueueFamilyIndices = GetDeviceQueueFamilies(m_VulkanPhysicalDevice->GetVulkanObject());
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

	if (vkCreateSwapchainKHR(m_VulkanLogicalDevice, &SwapChainCreateInfo, nullptr, &m_VulkanSwapChain) != VK_SUCCESS)
	{
		UNICA_LOG(Fatal, __FUNCTION__, "Failed to create VulkanSwapChain");
		return;
	}

	vkGetSwapchainImagesKHR(m_VulkanLogicalDevice, m_VulkanSwapChain, &SwapImageCount, nullptr);
	m_VulkanSwapChainImages.resize(SwapImageCount);
	vkGetSwapchainImagesKHR(m_VulkanLogicalDevice, m_VulkanSwapChain, &SwapImageCount, m_VulkanSwapChainImages.data());

	m_VulkanSwapChainImageFormat = SurfaceFormat.format;
	m_VulkanSwapChainExtent = Extent;
}

void VulkanAPI::CreateImageViews()
{
	uint32 SwapChainImageIteration = 0;
	m_VulkanSwapChainImageViews.resize(m_VulkanSwapChainImages.size());
	for (const VkImage& SwapChainImage : m_VulkanSwapChainImages)
	{
		VkImageViewCreateInfo ImageViewCreateInfo { };
		ImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		ImageViewCreateInfo.image = SwapChainImage;
		ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		ImageViewCreateInfo.format = m_VulkanSwapChainImageFormat;

		ImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		ImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		ImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		ImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		ImageViewCreateInfo.subresourceRange.levelCount = 1;
		ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		ImageViewCreateInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(m_VulkanLogicalDevice, &ImageViewCreateInfo, nullptr, &m_VulkanSwapChainImageViews[SwapChainImageIteration]) != VK_SUCCESS)
		{
			UNICA_LOG(Fatal, __FUNCTION__, "Failed to create VulkanImageViews");
			return;
		}

		SwapChainImageIteration++;
	}
}

void VulkanAPI::CreateRenderPass()
{
	VkAttachmentDescription VulkanColorAttachment { };
	VulkanColorAttachment.format = m_VulkanSwapChainImageFormat;
	VulkanColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	VulkanColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	VulkanColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	VulkanColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	VulkanColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	VulkanColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference VulkanColorAttachmentRef{};
	VulkanColorAttachmentRef.attachment = 0;
	VulkanColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription VulkanSubpass{};
	VulkanSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	VulkanSubpass.colorAttachmentCount = 1;
	VulkanSubpass.pColorAttachments = &VulkanColorAttachmentRef;

	VkRenderPassCreateInfo VulkanRenderPassCreateInfo{};
	VulkanRenderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	VulkanRenderPassCreateInfo.attachmentCount = 1;
	VulkanRenderPassCreateInfo.pAttachments = &VulkanColorAttachment;
	VulkanRenderPassCreateInfo.subpassCount = 1;
	VulkanRenderPassCreateInfo.pSubpasses = &VulkanSubpass;

	if (vkCreateRenderPass(m_VulkanLogicalDevice, &VulkanRenderPassCreateInfo, nullptr, &m_VulkanRenderPass) != VK_SUCCESS)
	{
		UNICA_LOG(Fatal, __FUNCTION__, "Failed to create VulkanRenderPass");
	}
}

void VulkanAPI::CreateGraphicsPipeline()
{
#if !UNICA_SHIPPING
	ShaderUtilities::CompileShaders();
#endif

	std::vector<char> VertShaderBinary = ShaderUtilities::LoadShader("Engine:Shaders/shader.vert");
	std::vector<char> FragShaderBinary = ShaderUtilities::LoadShader("Engine:Shaders/shader.frag");

	VkShaderModule VertShaderModule = CreateShaderModule(VertShaderBinary);
	VkShaderModule FragShaderModule = CreateShaderModule(FragShaderBinary);

	VkPipelineShaderStageCreateInfo VertPipelineShaderStageCreateInfo { };
	VertPipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	VertPipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	VertPipelineShaderStageCreateInfo.module = VertShaderModule;
	VertPipelineShaderStageCreateInfo.pName = "main";

	VkPipelineShaderStageCreateInfo FragPipelineShaderStageCreateInfo { };
	FragPipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	FragPipelineShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	FragPipelineShaderStageCreateInfo.module = FragShaderModule;
	FragPipelineShaderStageCreateInfo.pName = "main";

	VkPipelineShaderStageCreateInfo PipelineShaderStageCreateInfos[] = { VertPipelineShaderStageCreateInfo, FragPipelineShaderStageCreateInfo };

	std::vector<VkDynamicState> PipelineDynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	VkPipelineDynamicStateCreateInfo PipelineDynamicCreateInfo { };
	PipelineDynamicCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	PipelineDynamicCreateInfo.dynamicStateCount = static_cast<uint32_t>(PipelineDynamicStates.size());
	PipelineDynamicCreateInfo.pDynamicStates = PipelineDynamicStates.data();

	VkPipelineVertexInputStateCreateInfo PipelineVertexInputCreateInfo { };
	PipelineVertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	PipelineVertexInputCreateInfo.vertexBindingDescriptionCount = 0;
	PipelineVertexInputCreateInfo.pVertexBindingDescriptions = nullptr;
	PipelineVertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
	PipelineVertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;

	VkPipelineInputAssemblyStateCreateInfo PipelineInputAssemblyCreateInfo{};
	PipelineInputAssemblyCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	PipelineInputAssemblyCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	PipelineInputAssemblyCreateInfo.primitiveRestartEnable = VK_FALSE;

	VkViewport Viewport{};
	Viewport.x = 0.0f;
	Viewport.y = 0.0f;
	Viewport.width = static_cast<float>(m_VulkanSwapChainExtent.width);
	Viewport.height = static_cast<float>(m_VulkanSwapChainExtent.height);
	Viewport.minDepth = 0.0f;
	Viewport.maxDepth = 1.0f;

	VkRect2D ScissorRectangle{};
	ScissorRectangle.offset = {0, 0};
	ScissorRectangle.extent = m_VulkanSwapChainExtent;

	VkPipelineViewportStateCreateInfo PipelineViewportCreateInfo { };
	PipelineViewportCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	PipelineViewportCreateInfo.viewportCount = 1;
	PipelineViewportCreateInfo.pViewports = &Viewport;
	PipelineViewportCreateInfo.scissorCount = 1;
	PipelineViewportCreateInfo.pScissors = &ScissorRectangle;

	VkPipelineRasterizationStateCreateInfo PipelineRasterizationCreateInfo { };
	PipelineRasterizationCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	PipelineRasterizationCreateInfo.depthClampEnable = VK_FALSE;
	PipelineRasterizationCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	PipelineRasterizationCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	PipelineRasterizationCreateInfo.lineWidth = 1.0f;
	PipelineRasterizationCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	PipelineRasterizationCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	PipelineRasterizationCreateInfo.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo PipelineMultisampleCreateInfo { };
	PipelineMultisampleCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	PipelineMultisampleCreateInfo.sampleShadingEnable = VK_FALSE;
	PipelineMultisampleCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	PipelineMultisampleCreateInfo.minSampleShading = 1.0f;
	PipelineMultisampleCreateInfo.pSampleMask = nullptr;
	PipelineMultisampleCreateInfo.alphaToCoverageEnable = VK_FALSE;
	PipelineMultisampleCreateInfo.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState PipelineColorBlendAttachment { };
	PipelineColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	PipelineColorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo PipelineColorBlend { };
	PipelineColorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	PipelineColorBlend.logicOpEnable = VK_FALSE;
	PipelineColorBlend.attachmentCount = 1;
	PipelineColorBlend.pAttachments = &PipelineColorBlendAttachment;

	VkPipelineLayoutCreateInfo PipelineLayoutInfo { };
	PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	if (vkCreatePipelineLayout(m_VulkanLogicalDevice, &PipelineLayoutInfo, nullptr, &m_VulkanPipelineLayout) != VK_SUCCESS)
	{
		UNICA_LOG(Fatal, __FUNCTION__, "Failed to create a VulkanPipelineLayout");
	}

	VkGraphicsPipelineCreateInfo GraphicsPipelineCreateInfo { };
	GraphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	GraphicsPipelineCreateInfo.stageCount = 2;
	GraphicsPipelineCreateInfo.pStages = PipelineShaderStageCreateInfos;
	GraphicsPipelineCreateInfo.pVertexInputState = &PipelineVertexInputCreateInfo;
	GraphicsPipelineCreateInfo.pInputAssemblyState = &PipelineInputAssemblyCreateInfo;
	GraphicsPipelineCreateInfo.pViewportState = &PipelineViewportCreateInfo;
	GraphicsPipelineCreateInfo.pRasterizationState = &PipelineRasterizationCreateInfo;
	GraphicsPipelineCreateInfo.pMultisampleState = &PipelineMultisampleCreateInfo;
	GraphicsPipelineCreateInfo.pColorBlendState = &PipelineColorBlend;
	GraphicsPipelineCreateInfo.pDynamicState = &PipelineDynamicCreateInfo;
	GraphicsPipelineCreateInfo.layout = m_VulkanPipelineLayout;
	GraphicsPipelineCreateInfo.renderPass = m_VulkanRenderPass;
	GraphicsPipelineCreateInfo.subpass = 0;

	if (vkCreateGraphicsPipelines(m_VulkanLogicalDevice, VK_NULL_HANDLE, 1, &GraphicsPipelineCreateInfo, nullptr, &m_VulkanGraphicsPipeline) != VK_SUCCESS)
	{
		UNICA_LOG(Fatal, __FUNCTION__, "Failed to create the VulkanGraphicsPipeline");
	}

	// Cleanup shader modules since they've already been created
	vkDestroyShaderModule(m_VulkanLogicalDevice, VertShaderModule, nullptr);
	vkDestroyShaderModule(m_VulkanLogicalDevice, FragShaderModule, nullptr);
}

VkShaderModule VulkanAPI::CreateShaderModule(const std::vector<char>& ShaderBinary)
{
	VkShaderModuleCreateInfo ShaderModuleCreateInfo { };
	ShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	ShaderModuleCreateInfo.codeSize = ShaderBinary.size();
	ShaderModuleCreateInfo.pCode = reinterpret_cast<const uint32*>(ShaderBinary.data());

	VkShaderModule ShaderModule;
	if (vkCreateShaderModule(m_VulkanLogicalDevice, &ShaderModuleCreateInfo, nullptr, &ShaderModule) != VK_SUCCESS)
	{
		UNICA_LOG(Fatal, __FUNCTION__, "Failed to create a VulkanShaderModule");
	}
	
	return ShaderModule;
}

void VulkanAPI::CreateFramebuffers()
{
	uint32 SwapChainImageViewLoopIndex = 0;
	m_SwapChainFramebuffers.resize(m_VulkanSwapChainImageViews.size());
	for (VkImageView_T* SwapChainImageView : m_VulkanSwapChainImageViews)
	{
		const VkImageView ImageViewAttachments[] = { SwapChainImageView };
		VkFramebufferCreateInfo FramebufferCreateInfo { };
		FramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		FramebufferCreateInfo.renderPass = m_VulkanRenderPass;
		FramebufferCreateInfo.attachmentCount = 1;
		FramebufferCreateInfo.pAttachments = ImageViewAttachments;
		FramebufferCreateInfo.width = m_VulkanSwapChainExtent.width;
		FramebufferCreateInfo.height = m_VulkanSwapChainExtent.height;
		FramebufferCreateInfo.layers = 1;

		if (vkCreateFramebuffer(m_VulkanLogicalDevice, &FramebufferCreateInfo, nullptr, &m_SwapChainFramebuffers[SwapChainImageViewLoopIndex]) != VK_SUCCESS)
		{
			UNICA_LOG(Fatal, __FUNCTION__, "Failed to create a VulkanFramebuffer");
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

	if (vkCreateCommandPool(m_VulkanLogicalDevice, &CommandPoolCreateInfo, nullptr, &m_VulkanCommandPool) != VK_SUCCESS)
	{
		UNICA_LOG(Fatal, __FUNCTION__, "Failed to create the VulkanCommandPool");
	}
}

void VulkanAPI::CreateVulkanLogicalDevice()
{
	VulkanQueueFamilyIndices QueueFamilyIndices = GetDeviceQueueFamilies(m_VulkanPhysicalDevice->GetVulkanObject());
	if (!QueueFamilyIndices.WasSet())
	{
		UNICA_LOG(Fatal, __FUNCTION__, "No support for graphics and image presentation queues");
		return;
	}

	std::vector<VkDeviceQueueCreateInfo> QueueCreateInfos;
	std::set<uint32> UniqueQueueFamilies = { QueueFamilyIndices.GetGraphicsFamily().value(), QueueFamilyIndices.GetPresentImagesFamily().value() };
	
	const float QueuePriority = 1.f;
	for (const uint32 UniqueQueueFamily : UniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo QueueCreateInfo { };
		QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		QueueCreateInfo.queueFamilyIndex = UniqueQueueFamily;
		QueueCreateInfo.queueCount = 1;
		QueueCreateInfo.pQueuePriorities = &QueuePriority;
		QueueCreateInfos.push_back(QueueCreateInfo);
	}

	VkPhysicalDeviceFeatures DeviceFeatures { };

	VkDeviceCreateInfo DeviceCreateInfo { };
	DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	DeviceCreateInfo.queueCreateInfoCount = static_cast<uint32>(QueueCreateInfos.size());
	DeviceCreateInfo.pQueueCreateInfos = QueueCreateInfos.data();
	DeviceCreateInfo.pEnabledFeatures = &DeviceFeatures;

	DeviceCreateInfo.enabledExtensionCount = static_cast<uint32>(m_RequiredDeviceExtensions.size());
	DeviceCreateInfo.ppEnabledExtensionNames = m_RequiredDeviceExtensions.data();

	if (m_bValidationLayersEnabled)
	{
		DeviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_RequestedValidationLayers.size());
		DeviceCreateInfo.ppEnabledLayerNames = m_RequestedValidationLayers.data();
	}
	else
	{
		DeviceCreateInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(m_VulkanPhysicalDevice->GetVulkanObject(), &DeviceCreateInfo, nullptr, &m_VulkanLogicalDevice) != VK_SUCCESS)
	{
		UNICA_LOG(Fatal, __FUNCTION__, "Couldn't create a VulkanLogicalDevice");
		return;
	}
	
	vkGetDeviceQueue(m_VulkanLogicalDevice, QueueFamilyIndices.GetGraphicsFamily().value(), 0, &m_VulkanGraphicsQueue);
	vkGetDeviceQueue(m_VulkanLogicalDevice, QueueFamilyIndices.GetPresentImagesFamily().value(), 0, &m_VulkanPresentImagesQueue);
}

void VulkanAPI::Shutdown()
{
	vkDestroyCommandPool(m_VulkanLogicalDevice, m_VulkanCommandPool, nullptr);
	for (VkFramebuffer_T* SwapChainFramebuffer : m_SwapChainFramebuffers)
	{
		vkDestroyFramebuffer(m_VulkanLogicalDevice, SwapChainFramebuffer, nullptr);
	}
	vkDestroyPipeline(m_VulkanLogicalDevice, m_VulkanGraphicsPipeline, nullptr);
	vkDestroyPipelineLayout(m_VulkanLogicalDevice, m_VulkanPipelineLayout, nullptr);
	vkDestroyRenderPass(m_VulkanLogicalDevice, m_VulkanRenderPass, nullptr);
	for (const VkImageView& SwapChainImageView : m_VulkanSwapChainImageViews)
	{
		vkDestroyImageView(m_VulkanLogicalDevice, SwapChainImageView, nullptr);
	}
	vkDestroySwapchainKHR(m_VulkanLogicalDevice, m_VulkanSwapChain, nullptr);
	vkDestroyDevice(m_VulkanLogicalDevice, nullptr);
	m_VulkanWindowSurface->Destroy();
	m_VulkanInstance->Destroy();
}
