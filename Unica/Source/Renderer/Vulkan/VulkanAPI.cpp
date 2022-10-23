#include "VulkanAPI.h"

#include <map>
#include <vector>

#include "GLFW/glfw3.h"
#include "fmt/format.h"

#include "UnicaMinimal.h"

VulkanAPI::VulkanAPI()
{
	CreateVulkanInstance();
	CreateVulkanDebugMessenger();
	SelectVulkanPhysicalDevice();
}

void VulkanAPI::CreateVulkanInstance()
{
	VkApplicationInfo VulkanAppInfo{ };
	VulkanAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	VulkanAppInfo.pApplicationName = UnicaSettings::ApplicationName.c_str();
	VulkanAppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	VulkanAppInfo.pEngineName = UnicaSettings::EngineName.c_str();
	VulkanAppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	VulkanAppInfo.apiVersion = VK_API_VERSION_1_0;

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

	AddRequiredExtensions(VulkanCreateInfo, RequiredExtensions);
	AddValidationLayers(VulkanCreateInfo, VulkanDebugCreateInfo);

	if (vkCreateInstance(&VulkanCreateInfo, nullptr, &m_VulkanInstance) != VK_SUCCESS)
	{
		UNICA_LOG(Fatal, "LogRenderInterface", "Couldn't create Vulkan instance");
		return;
	}
	UNICA_LOG(Log, "LogRenderInterface", "Vulkan instance created");
}

void VulkanAPI::CreateVulkanDebugMessenger()
{
	if (!UnicaSettings::bValidationLayersEnabled)
	{
		return;
	}

	VkDebugUtilsMessengerCreateInfoEXT VulkanCreateInfo;
	PopulateVulkanDebugMessengerInfo(VulkanCreateInfo);

	if (CreateVulkanDebugUtilsMessenger(m_VulkanInstance, &VulkanCreateInfo, nullptr, &m_VulkanDebugMessenger) != VK_SUCCESS)
	{
		UNICA_LOG(Error, "LogRenderInterface", "Couldn't setup VulkanDebugMessenger");
	}
}

void VulkanAPI::SelectVulkanPhysicalDevice()
{
	uint32 VulkanPhysicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(m_VulkanInstance, &VulkanPhysicalDeviceCount, nullptr);
	if (VulkanPhysicalDeviceCount < 1)
	{
		UNICA_LOG(Fatal, "LogRenderInterface", "No GPUs with Vulkan support found");
	}

	std::vector<VkPhysicalDevice> VulkanPhysicalDevices(VulkanPhysicalDeviceCount);
	vkEnumeratePhysicalDevices(m_VulkanInstance, &VulkanPhysicalDeviceCount, VulkanPhysicalDevices.data());

	std::multimap<uint32, VkPhysicalDevice> VulkanPhysicalDeviceCandidates;
	for (const VkPhysicalDevice& VulkanPhysicalDevice : VulkanPhysicalDevices)
	{
		uint32 VulkanPhysicalDeviceScore = RateVulkanPhysicalDevice(VulkanPhysicalDevice);
		VulkanPhysicalDeviceCandidates.insert(std::make_pair(VulkanPhysicalDeviceScore, VulkanPhysicalDevice));
	}

	if (VulkanPhysicalDeviceCandidates.rbegin()->first > 0)
	{
		m_VulkanPhysicalDevice = VulkanPhysicalDeviceCandidates.rbegin()->second;
	}
	else
	{
		UNICA_LOG(Fatal, "LogRenderInterface", "No suitable GPU found");
	}
}

uint32 VulkanAPI::RateVulkanPhysicalDevice(const VkPhysicalDevice& VulkanPhysicalDevice)
{
	uint32 Score = 1;

	VkPhysicalDeviceProperties VulkanPhysicalDeviceProperties;
	VkPhysicalDeviceFeatures VulkanPhysicalDeviceFeatures;
	vkGetPhysicalDeviceProperties(VulkanPhysicalDevice, &VulkanPhysicalDeviceProperties);
	vkGetPhysicalDeviceFeatures(VulkanPhysicalDevice, &VulkanPhysicalDeviceFeatures);

	if (VulkanPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		Score += 1000;
	}
	
	return Score;
}

void VulkanAPI::PopulateVulkanDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT& VulkanCreateInfo)
{
	VulkanCreateInfo = { };
	VulkanCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	VulkanCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	VulkanCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	VulkanCreateInfo.pfnUserCallback = VulkanDebugCallback;
}

void VulkanAPI::AddRequiredExtensions(VkInstanceCreateInfo& VulkanCreateInfo, std::vector<const char*>& RequiredExtensions)
{
    
#ifdef __APPLE__
    RequiredExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
    RequiredExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif // __APPLE__
    
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

void VulkanAPI::AddValidationLayers(VkInstanceCreateInfo& VulkanCreateInfo, VkDebugUtilsMessengerCreateInfoEXT& VulkanDebugCreateInfo)
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

	PopulateVulkanDebugMessengerInfo(VulkanDebugCreateInfo);
	VulkanCreateInfo.pNext = &VulkanDebugCreateInfo;
}

VkResult VulkanAPI::CreateVulkanDebugUtilsMessenger(VkInstance VulkanInstance, const VkDebugUtilsMessengerCreateInfoEXT* VulkanCreateInfo, const VkAllocationCallbacks* VulkanAllocator, VkDebugUtilsMessengerEXT* VulkanDebugMessenger)
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

void VulkanAPI::DestroyVulkanDebugUtilsMessengerEXT(VkInstance VulkanInstance, VkDebugUtilsMessengerEXT VulkanDebugMessenger, const VkAllocationCallbacks* VulkanAllocator)
{
	const auto Func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(VulkanInstance, "vkDestroyDebugUtilsMessengerEXT"));
	if (Func != nullptr)
	{
		Func(VulkanInstance, VulkanDebugMessenger, VulkanAllocator);
	}
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanAPI::VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity, VkDebugUtilsMessageTypeFlagsEXT MessageType, const VkDebugUtilsMessengerCallbackDataEXT* CallbackData, void* UserData)
{
	LogLevel LogLvl = Log;

	if ((MessageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		LogLvl = Warning;
	}
	else if ((MessageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{
		LogLvl = Error;
	}

	UNICA_LOG(LogLvl, "LogVulkanAPI", CallbackData->pMessage);
	return VK_FALSE;
}

VulkanAPI::~VulkanAPI()
{
	if (UnicaSettings::bValidationLayersEnabled)
	{
		DestroyVulkanDebugUtilsMessengerEXT(m_VulkanInstance, m_VulkanDebugMessenger, nullptr);
	}
	vkDestroyInstance(m_VulkanInstance, nullptr);
	UNICA_LOG(Log, "LogRenderInterface", "Vulkan instance has been destroyed");
}

