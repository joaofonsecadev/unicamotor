// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include "SDL3/SDL.h"

class RenderWindow
{
public:
    RenderWindow();
    ~RenderWindow();

    void Tick();

    void HandleSdlEvent(const SDL_Event& Event);

    SDL_Window* GetSdlWindow() const { return m_SdlWindow; }
    bool GetWindowResized() const { return m_bWindowResized; }
    bool GetWindowIsMinimized() const { return m_bWindowIsMinimized; }

    void SetWindowResized(const bool Value) { m_bWindowResized = Value; }

private:
    SDL_Window* m_SdlWindow = nullptr;
    
    bool m_bWindowResized = false;
    bool m_bWindowIsMinimized = false;
};
