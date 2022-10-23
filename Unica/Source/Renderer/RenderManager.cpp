// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "RenderManager.h"

void RenderManager::Init()
{
    m_RenderWindow = std::make_unique<RenderWindow>();
    m_RenderInterface = std::make_unique<VulkanAPI>();
}

void RenderManager::Tick()
{
    m_RenderWindow->Tick();
}
