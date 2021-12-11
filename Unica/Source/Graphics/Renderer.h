// Copyright João Fonseca, All Rights Reserved.

#pragma once

#include "SDL_render.h"
#include "SDL_video.h"

#include "Engine/EngineCoreComponent.h"

class UnicaEngine;

class Renderer : public EngineCoreComponent
{
public:
    Renderer() { }
    void Init(UnicaEngine* EngineInstance) override;

    SDL_Window* GetSDLWindow() const { return m_SDLWindow; }
    SDL_Renderer* GetSDLRenderer() const { return m_SDLRenderer; }

private:
    SDL_Window* m_SDLWindow = nullptr;
    SDL_Renderer* m_SDLRenderer = nullptr;
};
