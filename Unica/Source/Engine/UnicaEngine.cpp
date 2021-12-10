// Copyright João Fonseca, All Rights Reserved.

#include "UnicaEngine.h"

#include "SDL.h"

#include "Log/Log.h"

void UnicaEngine::Init()
{
    Logger::Log(Log, "Initiating engine...");

    m_Renderer = Renderer();
    m_EventHandler = EventHandler();

    m_EventHandler.Init(this);
    m_Renderer.Init(this);
}

void UnicaEngine::Tick()
{
    m_EventHandler.Tick();
    m_Renderer.Tick();
}

void UnicaEngine::Exit()
{
    Logger::Log(Log, "Exiting engine...");
    
    SDL_DestroyWindow(m_Renderer.GetSDLWindow());
    SDL_DestroyRenderer(m_Renderer.GetSDLRenderer());
    SDL_Quit();
}
