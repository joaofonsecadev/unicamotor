// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "RenderManager.h"

#include "UnicaMinimal.h"
#include "Vulkan/VulkanInterface.h"

void RenderManager::Init()
{
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
