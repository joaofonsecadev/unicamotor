// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "RenderManager.h"

void RenderManager::Init()
{
    m_GameWindow = std::make_unique<GameWindow>();
}

void RenderManager::Tick()
{
    m_GameWindow->Tick();
}
