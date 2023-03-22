﻿#pragma once

#include "VulkanObjectInterface.h"
#include "VulkanAPI.h"

class VulkanInstance : public VulkanObjectInterface<VkInstance>
{
public:
    VulkanInstance(VulkanAPI* OwningVulkanAPI) : VulkanObjectInterface(OwningVulkanAPI) { }
    
    void Init() override;
    void Destroy() override;

    ~VulkanInstance() override = default;

private:
    void CreateVulkanDebugMessenger();
    static void PopulateVulkanDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT& VulkanCreateInfo);
    
    void AddRequiredExtensions(VkInstanceCreateInfo& VulkanCreateInfo, std::vector<const char*>& RequiredExtensions) const;
    void AddValidationLayers(VkInstanceCreateInfo& VulkanCreateInfo, VkDebugUtilsMessengerCreateInfoEXT& VulkanDebugCreateInfo) const;
    
    static VkResult CreateVulkanDebugUtilsMessenger(VkInstance VulkanInstance,
        const VkDebugUtilsMessengerCreateInfoEXT* VulkanCreateInfo,
        const VkAllocationCallbacks* VulkanAllocator,
        VkDebugUtilsMessengerEXT* VulkanDebugMessenger
    );
    static void DestroyVulkanDebugUtilsMessengerEXT(
        VkInstance VulkanInstance,
        VkDebugUtilsMessengerEXT VulkanDebugMessenger,
        const VkAllocationCallbacks* VulkanAllocator
    );
    static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT MessageType,
        const VkDebugUtilsMessengerCallbackDataEXT* CallbackData,
        void* UserData
    );

    VkDebugUtilsMessengerEXT m_VulkanDebugMessenger = VK_NULL_HANDLE;
};
