// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "RenderInterface.h"

#include <vector>

#include "GLFW/glfw3.h"
#include "fmt/format.h"

#include "UnicaMinimal.h"

RenderInterface::RenderInterface()
{
    CreateVulkanInstance();
}

void RenderInterface::CreateVulkanInstance()
{
    VkApplicationInfo VulkanAppInfo { };
    VulkanAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    VulkanAppInfo.pApplicationName = UnicaSettings::ApplicationName.c_str();
    VulkanAppInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    VulkanAppInfo.pEngineName =UnicaSettings::EngineName.c_str();
    VulkanAppInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
    VulkanAppInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo VulkanCreateInfo { };
    VulkanCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    VulkanCreateInfo.pApplicationInfo = &VulkanAppInfo;
    VulkanCreateInfo.enabledLayerCount = 0;

	uint32 GlfwExtensionCount = 0;
	const char** GlfwExtensions = glfwGetRequiredInstanceExtensions(&GlfwExtensionCount);
	std::vector<const char*> RequiredExtensions(GlfwExtensions, GlfwExtensions + GlfwExtensionCount);

    AddRequiredExtensions(VulkanCreateInfo, RequiredExtensions);
    AddValidationLayers(VulkanCreateInfo);

    if (vkCreateInstance(&VulkanCreateInfo, nullptr, &m_VulkanInstance) != VK_SUCCESS)
    {
        UNICA_LOG(Fatal, "LogRenderInterface", "Couldn't create Vulkan instance");
        return;
    }
    UNICA_LOG(Log, "LogRenderInterface", "Vulkan instance created");
}

void RenderInterface::AddRequiredExtensions(VkInstanceCreateInfo& VulkanCreateInfo, std::vector<const char*>& RequiredExtensions)
{
    if (UnicaSettings::bValidationLayersEnabled)
    {
        RequiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    uint32 AvailableExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &AvailableExtensionCount, nullptr);
	std::vector<VkExtensionProperties> AvailableExtensions(AvailableExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &AvailableExtensionCount, AvailableExtensions.data());

    bool bAllExtensionsFound = true;
    for (const char* RequiredExtensionName : RequiredExtensions)
    {
        bool bWasExtensionFound = false;
        for (const VkExtensionProperties& AvailableExtension : AvailableExtensions)
        {
            if (strcmp(RequiredExtensionName, AvailableExtension.extensionName) == 0)
            {
                bWasExtensionFound = true;
                break;
            }
        }
        if (!bWasExtensionFound)
        {
            UNICA_LOG(Error, "LogRenderInterface", fmt::format("Graphic instance extension \"{}\" not found", RequiredExtensionName));
            bAllExtensionsFound = false;
        }
    }
    if (!bAllExtensionsFound)
    {
		UNICA_LOG(Fatal, "LogRenderInterface", "Not all required instance extensions found");
		return;
    }

    VulkanCreateInfo.enabledExtensionCount = static_cast<uint32>(RequiredExtensions.size());
    VulkanCreateInfo.ppEnabledExtensionNames = RequiredExtensions.data();
}

void RenderInterface::AddValidationLayers(VkInstanceCreateInfo& VulkanCreateInfo)
{
    if (!UnicaSettings::bValidationLayersEnabled)
    {
        return;
    }

    uint32 LayerCount;
    vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

    std::vector<VkLayerProperties> AvailableLayers(LayerCount);
    vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers.data());

    bool bAllLayersFound = true;
    for (const char* RequestedLayerName : UnicaSettings::RequestedValidationLayers)
    {
        bool bWasLayerFound = false;
        for (const VkLayerProperties& AvailableLayer : AvailableLayers)
        {
            if (strcmp(RequestedLayerName, AvailableLayer.layerName) == 0)
            {
                bWasLayerFound = true;
                break;
            }
        }
        if (!bWasLayerFound)
        {
			UNICA_LOG(Error, "LogRenderInterface", fmt::format("VulkanValidationLayer \"{}\" not found", RequestedLayerName));
			bAllLayersFound = false;
        }
    }
    if (!bAllLayersFound)
	{
        UNICA_LOG(Error, "LogRenderInterface", "Won't enable VulkanValidationLayers since not all of them are available");
        return;
	}

	VulkanCreateInfo.enabledLayerCount = static_cast<uint32>(UnicaSettings::RequestedValidationLayers.size());
	VulkanCreateInfo.ppEnabledLayerNames = UnicaSettings::RequestedValidationLayers.data();
}

RenderInterface::~RenderInterface()
{
    vkDestroyInstance(m_VulkanInstance, nullptr);
    UNICA_LOG(Log, "LogRenderInterface", "Vulkan instance has been destroyed");
}
