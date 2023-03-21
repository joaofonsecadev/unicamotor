// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "RenderManager.h"
#include "UnicaMinimal.h"
#include "SDL/SdlAPI.h"
#include "Vulkan/VulkanAPI.h"

void RenderManager::Init()
{
    m_RenderInterface = std::make_unique_for_overwrite<VulkanAPI>();
    m_RenderInterface->Init();
}

void RenderManager::Tick()
{
    UNICA_PROFILE_FUNCTION
    m_RenderInterface->Tick();
}

void RenderManager::Shutdown()
{
    m_RenderInterface->Shutdown();
}
