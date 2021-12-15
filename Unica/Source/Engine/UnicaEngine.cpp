// Copyright João Fonseca, All Rights Reserved.

#include "UnicaPch.h"
#include "UnicaEngine.h"

void UnicaEngine::Init()
{
    Logger::Log(Log, "Initiating engine...");

    m_EventHandler.Init(this);
    m_Renderer.Init(this);
}

void UnicaEngine::Tick()
{
    m_EventHandler.Tick();
    m_Renderer.Tick();
    RequestExit();
}

void UnicaEngine::Exit()
{
    Logger::Log(Log, "Exiting engine...");
    
}
