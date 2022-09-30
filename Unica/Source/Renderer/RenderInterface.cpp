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

    uint32 GlfwExtensionCount = 0;
    const char** GlfwExtensions = glfwGetRequiredInstanceExtensions(&GlfwExtensionCount);

    VulkanCreateInfo.enabledExtensionCount = GlfwExtensionCount;
    VulkanCreateInfo.ppEnabledExtensionNames = GlfwExtensions;
    VulkanCreateInfo.enabledLayerCount = 0;

	if (UnicaSettings::bValidationLayersEnabled)
	{
        if (!CheckAvailableValidationLayers())
        {
			VulkanCreateInfo.enabledLayerCount = static_cast<uint32>(UnicaSettings::RequestedValidationLayers.size());
			VulkanCreateInfo.ppEnabledLayerNames = UnicaSettings::RequestedValidationLayers.data();
        }
        else
        {
			UNICA_LOG(Fatal, "LogRenderInterface", "Not all the requested Vulkan Validation Layers are available");
        }
	}

    if (vkCreateInstance(&VulkanCreateInfo, nullptr, &m_VulkanInstance) != VK_SUCCESS)
    {
        UNICA_LOG(Fatal, "LogRenderInterface", "Couldn't create Vulkan instance");
        return;
    }
    UNICA_LOG(Log, "LogRenderInterface", "Vulkan instance created successfully");
}

bool RenderInterface::CheckAvailableValidationLayers()
{
    uint32 LayerCount;
    vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

    std::vector<VkLayerProperties> AvailableLayers(LayerCount);
    vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers.data());

    for (const char* LayerName : UnicaSettings::RequestedValidationLayers)
    {
        bool bLayerFound = false;
        for (const VkLayerProperties& LayerProperties : AvailableLayers)
        {
            if (strcmp(LayerName, LayerProperties.layerName) == 0)
            {
                bLayerFound = true;
                break;
            }
        }
        if (!bLayerFound)
        {
            return false;
        }
    }

    return true;
}

void RenderInterface::LogVulkanInstanceExtensions()
{
    uint32 ExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, nullptr);
    std::vector<VkExtensionProperties> ExtensionList(ExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, ExtensionList.data());

    std::string ExtensionLogString = "Listing available Vulkan extensions:";
    for (const VkExtensionProperties& Extension : ExtensionList)
    {
        ExtensionLogString += fmt::format("\n\t{}", Extension.extensionName);
    }

    UNICA_LOG(Log, "LogRenderInterface", ExtensionLogString);
}

RenderInterface::~RenderInterface()
{
    vkDestroyInstance(m_VulkanInstance, nullptr);
    UNICA_LOG(Log, "LogRenderInterface", "Vulkan instance has been destroyed");
}
