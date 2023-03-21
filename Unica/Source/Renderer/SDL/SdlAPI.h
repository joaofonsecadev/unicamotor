#pragma once

#include <SDL_video.h>

#include "Renderer/RenderInterface.h"

class SdlAPI : public RenderInterface
{
public:
    void Init() override;
    void Tick() override;
    void Shutdown() override;

private:
    void SdlPoolEvents();
    
    SDL_Window* m_SdlWindow = nullptr;
    SDL_Surface* m_SdlWindowSurface = nullptr;
};
