// Copyright João Fonseca, All Rights Reserved.

#pragma once

#include "SDL_render.h"
#include "SDL_video.h"
#include "Interfaces/EngineCoreComponent.h"

class UnicaEngine;

class Renderer : IEngineCoreComponent
{
public:
    Renderer() { }
    ~Renderer() override { }
    void Init(UnicaEngine* const EngineInstance);
    void Tick() override { }

private:
    UnicaEngine* m_EngineInstance = nullptr;

    SDL_Window* m_Window = nullptr;
    SDL_Renderer* m_Renderer = nullptr;
};
