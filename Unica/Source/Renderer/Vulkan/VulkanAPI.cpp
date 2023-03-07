// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "VulkanAPI.h"

#include <map>
#include <set>
#include <vector>

#include "GLFW/glfw3.h"
#include "fmt/format.h"

#include "UnicaMinimal.h"
#include "VulkanQueueFamilyIndices.h"
#include "Renderer/RenderManager.h"

VulkanAPI::VulkanAPI(const RenderManager* OwningRenderManager)
{
	m_OwningRenderManager = OwningRenderManager;
	
	CreateVulkanInstance();
	CreateVulkanDebugMessenger();
	CreateVulkanWindowSurface();
	SelectVulkanPhysicalDevice();
	CreateVulkanLogicalDevice();
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

	if (UnicaSettings::bValidationLayersEnabled)
	{
		RequiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	AddRequiredExtensions(VulkanCreateInfo, RequiredExtensions);
	AddValidationLayers(VulkanCreateInfo, VulkanDebugCreateInfo);

	if (vkCreateInstance(&VulkanCreateInfo, nullptr, &m_VulkanInstance) != VK_SUCCESS)
	{
		UNICA_LOG(Fatal, "LogVulkanAPI", "Couldn't create Vulkan instance");
		return;
	}
	UNICA_LOG(Log, "LogVulkanAPI", "Vulkan instance created");
}

void VulkanAPI::SelectVulkanPhysicalDevice()
{
	uint32 VulkanPhysicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(m_VulkanInstance, &VulkanPhysicalDeviceCount, nullptr);
	if (VulkanPhysicalDeviceCount < 1)
	{
		UNICA_LOG(Fatal, "LogVulkanAPI", "No GPUs with Vulkan support found");
		return;
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
		UNICA_LOG(Fatal, "LogVulkanAPI", "No suitable GPU found");
		return;
	}
}

uint32 VulkanAPI::RateVulkanPhysicalDevice(const VkPhysicalDevice& VulkanPhysicalDevice)
{
	if (!GetDeviceQueueFamilies(VulkanPhysicalDevice).WasSet())
	{
		return 0;
	}

	if (!DeviceHasRequiredExtensions(VulkanPhysicalDevice))
	{
		return 0;
	}
	
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

bool VulkanAPI::DeviceHasRequiredExtensions(const VkPhysicalDevice& VulkanPhysicalDevice)
{
	const std::vector<const char*> RequiredDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	
	uint32_t AvailableDeviceExtensionCount;
	vkEnumerateDeviceExtensionProperties(VulkanPhysicalDevice, nullptr, &AvailableDeviceExtensionCount, nullptr);
	std::vector<VkExtensionProperties> AvailableDeviceExtensions(AvailableDeviceExtensionCount);
	vkEnumerateDeviceExtensionProperties(VulkanPhysicalDevice, nullptr, &AvailableDeviceExtensionCount, AvailableDeviceExtensions.data());

	std::set<std::string> DeviceExtensions(RequiredDeviceExtensions.begin(), RequiredDeviceExtensions.end());
	for (const VkExtensionProperties& DeviceExtension : AvailableDeviceExtensions)
	{
		DeviceExtensions.erase(DeviceExtension.extensionName);
	}

	return DeviceExtensions.empty();
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
		vkGetPhysicalDeviceSurfaceSupportKHR(VulkanPhysicalDevice, QueueFamilyIndex, m_VulkanWindowSurface, &PresentImagesSupport);

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

void VulkanAPI::CreateVulkanLogicalDevice()
{
	VulkanQueueFamilyIndices QueueFamilyIndices = GetDeviceQueueFamilies(m_VulkanPhysicalDevice);
	if (!QueueFamilyIndices.WasSet())
	{
		UNICA_LOG(Fatal, "LogVulkanAPI", "No support for graphics and image presentation queues");
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

	if (vkCreateDevice(m_VulkanPhysicalDevice, &DeviceCreateInfo, nullptr, &m_VulkanLogicalDevice) != VK_SUCCESS)
	{
		UNICA_LOG(Fatal, "LogVulkanAPI", "Couldn't create a VulkanLogicalDevice");
		return;
	}
	vkGetDeviceQueue(m_VulkanLogicalDevice, QueueFamilyIndices.GetGraphicsFamily().value(), 0, &m_VulkanGraphicsQueue);
	vkGetDeviceQueue(m_VulkanLogicalDevice, QueueFamilyIndices.GetPresentImagesFamily().value(), 0, &m_VulkanPresentImagesQueue);
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
		UNICA_LOG(Error, "LogVulkanAPI", "Couldn't setup VulkanDebugMessenger");
	}
}

void VulkanAPI::CreateVulkanWindowSurface()
{
	if (glfwCreateWindowSurface(m_VulkanInstance, m_OwningRenderManager->GetRenderWindow()->GetGlfwWindow(), nullptr, &m_VulkanWindowSurface))
	{
		UNICA_LOG(Fatal, "LogVulkanAPI", "Failed to create VulkanWindowSurface");
		return;
	}
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
			UNICA_LOG(Error, "LogVulkanAPI", fmt::format("Graphic instance extension \"{}\" not found", RequiredExtensionName));
			bAllExtensionsFound = false;
		}
	}
	if (!bAllExtensionsFound)
	{
		UNICA_LOG(Fatal, "LogVulkanAPI", "Not all required instance extensions found");
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
			UNICA_LOG(Error, "LogVulkanAPI", fmt::format("VulkanValidationLayer \"{}\" not found", RequestedLayerName));
			bAllLayersFound = false;
		}
	}
	if (!bAllLayersFound)
	{
		UNICA_LOG(Error, "LogVulkanAPI", "Won't enable VulkanValidationLayers since not all of them are available");
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
	vkDestroyDevice(m_VulkanLogicalDevice, nullptr);
	vkDestroySurfaceKHR(m_VulkanInstance, m_VulkanWindowSurface, nullptr);
	if (UnicaSettings::bValidationLayersEnabled)
	{
		DestroyVulkanDebugUtilsMessengerEXT(m_VulkanInstance, m_VulkanDebugMessenger, nullptr);
	}
	vkDestroyInstance(m_VulkanInstance, nullptr);
	UNICA_LOG(Log, "LogVulkanAPI", "Vulkan instance has been destroyed");
}

