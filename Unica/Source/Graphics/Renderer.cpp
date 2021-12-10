// Copyright João Fonseca, All Rights Reserved.

#include "Renderer.h"
#include "SDL.h"
#include "Log/Log.h"

void Renderer::Init(UnicaEngine* const EngineInstance)
{
    Logger::Log(Log, "Initiating renderer...");
    
    m_EngineInstance = EngineInstance;

    SDL_InitSubSystem(SDL_INIT_VIDEO);
    m_SDLWindow = SDL_CreateWindow("UnicaEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, SDL_WINDOW_VULKAN);
    m_SDLRenderer = SDL_CreateRenderer(m_SDLWindow, -1, SDL_RENDERER_ACCELERATED);
}
