// 2021-2023 Copyright joaofonseca.dev, All Rights Reserved.

﻿#pragma once

struct VulkanSwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR SurfaceCapabilities;
    std::vector<VkSurfaceFormatKHR> SurfaceFormats;
    std::vector<VkPresentModeKHR> PresentModes;
};
