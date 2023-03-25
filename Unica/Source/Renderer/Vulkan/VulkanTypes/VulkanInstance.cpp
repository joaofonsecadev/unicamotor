// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "VulkanInstance.h"

#include <fmt/format.h>

void VulkanInstance::Init()
{
    VkApplicationInfo VulkanAppInfo{ };
    VulkanAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    VulkanAppInfo.pApplicationName = UnicaSettings::ApplicationName.c_str();
    VulkanAppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    VulkanAppInfo.pEngineName = UnicaSettings::EngineName.c_str();
    VulkanAppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    VulkanAppInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo VulkanCreateInfo{ };
    VkDebugUtilsMessengerCreateInfoEXT VulkanDebugCreateInfo;
    VulkanCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    VulkanCreateInfo.pApplicationInfo = &VulkanAppInfo;
    VulkanCreateInfo.enabledLayerCount = 0;
    VulkanCreateInfo.pNext = nullptr;
    
#ifdef __APPLE__
    VulkanCreateInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif // __APPLE__

    uint32 GlfwExtensionCount = 0;
    const char** GlfwExtensions = glfwGetRequiredInstanceExtensions(&GlfwExtensionCount);
    std::vector<const char*> RequiredExtensions(GlfwExtensions, GlfwExtensions + GlfwExtensionCount);

    if (m_OwningVulkanAPI->GetValidationLayersEnabled())
    {
        RequiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    AddRequiredExtensions(VulkanCreateInfo, RequiredExtensions);
    AddValidationLayers(VulkanCreateInfo, VulkanDebugCreateInfo);

    if (vkCreateInstance(&VulkanCreateInfo, nullptr, &m_VulkanObject) != VK_SUCCESS)
    {
        UNICA_LOG(spdlog::level::critical, "Couldn't create Vulkan instance");
    }
    UNICA_LOG(spdlog::level::info, "Vulkan instance created");

    CreateVulkanDebugMessenger();
}

void VulkanInstance::AddRequiredExtensions(VkInstanceCreateInfo& VulkanCreateInfo, std::vector<const char*>& RequiredExtensions) const
{
#ifdef __APPLE__
    RequiredExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    RequiredExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif // __APPLE__

    uint32 AvailableInstanceExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &AvailableInstanceExtensionCount, nullptr);
    std::vector<VkExtensionProperties> AvailableInstanceExtensions(AvailableInstanceExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &AvailableInstanceExtensionCount, AvailableInstanceExtensions.data());

    bool bAllExtensionsFound = true;
    for (const char* RequiredExtensionName : RequiredExtensions)
    {
        bool bWasExtensionFound = false;
        for (const VkExtensionProperties& AvailableExtension : AvailableInstanceExtensions)
        {
            if (strcmp(RequiredExtensionName, AvailableExtension.extensionName) == 0)
            {
                bWasExtensionFound = true;
                break;
            }
        }
        if (!bWasExtensionFound)
        {
            UNICA_LOG(spdlog::level::err, fmt::format("Graphic instance extension \"{}\" not found", RequiredExtensionName));
            bAllExtensionsFound = false;
        }
    }
    if (!bAllExtensionsFound)
    {
        UNICA_LOG(spdlog::level::critical, "Not all required instance extensions found");
        return;
    }

    VulkanCreateInfo.enabledExtensionCount = static_cast<uint32>(RequiredExtensions.size());
    VulkanCreateInfo.ppEnabledExtensionNames = RequiredExtensions.data();
}

void VulkanInstance::AddValidationLayers(VkInstanceCreateInfo& VulkanCreateInfo, VkDebugUtilsMessengerCreateInfoEXT& VulkanDebugCreateInfo) const
{
    if (!m_OwningVulkanAPI->GetValidationLayersEnabled())
    {
        return;
    }

    uint32 LayerCount;
    vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);

    std::vector<VkLayerProperties> AvailableLayers(LayerCount);
    vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers.data());

    bool bAllLayersFound = true;
    for (const char* RequestedLayerName : m_OwningVulkanAPI->GetRequestedValidationLayers())
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
            UNICA_LOG(spdlog::level::err, fmt::format("VulkanValidationLayer \"{}\" not found", RequestedLayerName));
            bAllLayersFound = false;
        }
    }
    if (!bAllLayersFound)
    {
        UNICA_LOG(spdlog::level::err, "Won't enable VulkanValidationLayers since not all of them are available");
        return;
    }

    VulkanCreateInfo.enabledLayerCount = static_cast<uint32>(m_OwningVulkanAPI->GetRequestedValidationLayers().size());
    VulkanCreateInfo.ppEnabledLayerNames = m_OwningVulkanAPI->GetRequestedValidationLayers().data();

    PopulateVulkanDebugMessengerInfo(VulkanDebugCreateInfo);
    VulkanCreateInfo.pNext = &VulkanDebugCreateInfo;
}

void VulkanInstance::CreateVulkanDebugMessenger()
{
    if (m_OwningVulkanAPI->GetValidationLayersEnabled())
    {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT VulkanCreateInfo;
    PopulateVulkanDebugMessengerInfo(VulkanCreateInfo);

    if (CreateVulkanDebugUtilsMessenger(m_VulkanObject, &VulkanCreateInfo, nullptr, &m_VulkanDebugMessenger) != VK_SUCCESS)
    {
        UNICA_LOG(spdlog::level::err, "Couldn't setup VulkanDebugMessenger");
    }
}

VkResult VulkanInstance::CreateVulkanDebugUtilsMessenger(VkInstance VulkanInstance, const VkDebugUtilsMessengerCreateInfoEXT* VulkanCreateInfo, const VkAllocationCallbacks* VulkanAllocator, VkDebugUtilsMessengerEXT* VulkanDebugMessenger)
{
    const auto Func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(VulkanInstance, "vkCreateDebugUtilsMessengerEXT"));
    if (Func != nullptr)
    {
        return Func(VulkanInstance, VulkanCreateInfo, VulkanAllocator, VulkanDebugMessenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void VulkanInstance::DestroyVulkanDebugUtilsMessengerEXT(VkInstance VulkanInstance, VkDebugUtilsMessengerEXT VulkanDebugMessenger, const VkAllocationCallbacks* VulkanAllocator)
{
    const auto Func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(VulkanInstance, "vkDestroyDebugUtilsMessengerEXT"));
    if (Func != nullptr)
    {
        Func(VulkanInstance, VulkanDebugMessenger, VulkanAllocator);
    }
}

void VulkanInstance::PopulateVulkanDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT& VulkanCreateInfo)
{
    VulkanCreateInfo = { };
    VulkanCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    VulkanCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    VulkanCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    VulkanCreateInfo.pfnUserCallback = VulkanDebugCallback;
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity, VkDebugUtilsMessageTypeFlagsEXT MessageType, const VkDebugUtilsMessengerCallbackDataEXT* CallbackData, void* UserData)
{
    if ((MessageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        UNICA_LOG_WARN(CallbackData->pMessage);
    }
    else if ((MessageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        UNICA_LOG_ERROR(CallbackData->pMessage);
    }

    return VK_FALSE;
}

void VulkanInstance::Destroy()
{
    if (m_OwningVulkanAPI->GetValidationLayersEnabled())
    {
        DestroyVulkanDebugUtilsMessengerEXT(m_VulkanObject, m_VulkanDebugMessenger, nullptr);
    }
    vkDestroyInstance(m_VulkanObject, nullptr);
}
