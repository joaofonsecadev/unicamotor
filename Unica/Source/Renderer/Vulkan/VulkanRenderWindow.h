// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include "SDL3/SDL.h"

class SdlRenderWindow
{
public:
    SdlRenderWindow();
    ~SdlRenderWindow();

    void Tick();

    SDL_Window* GetSdlWindow() const { return m_SdlWindow; }

private:
    SDL_Window* m_SdlWindow;
};
