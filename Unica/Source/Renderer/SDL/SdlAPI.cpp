// 2021-2023 Copyright joaofonseca.dev, All Rights Reserved.

﻿#include "SdlAPI.h"

#include "UnicaInstance.h"
#include "Log/Logger.h"
#include "SDL3/SDL.h"

void SdlAPI::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        UNICA_LOG(Fatal, __FUNCTION__, "Couldn't initialize SDL");
        return;
    }

    m_SdlWindow = SDL_CreateWindow(UnicaSettings::ApplicationName.c_str(), UnicaSettings::WindowWidth, UnicaSettings::WindowHeight, 0);
    if (m_SdlWindow == nullptr)
    {
        UNICA_LOG(Fatal, __FUNCTION__, "SDL window creation failed");
        return;
    }

    m_SdlWindowSurface = SDL_GetWindowSurface(m_SdlWindow);
}

void SdlAPI::Tick()
{
    UNICA_PROFILE_FUNCTION
    SdlPoolEvents();
}

void SdlAPI::SdlPoolEvents()
{
    UNICA_PROFILE_FUNCTION
    SDL_Event SdlEvent;
    SDL_PollEvent(&SdlEvent);
    if (SdlEvent.type == SDL_EVENT_QUIT)
    {
        UnicaInstance::RequestExit();
    }
}

void SdlAPI::Shutdown()
{
    SDL_DestroyWindow(m_SdlWindow);
    SDL_Quit();
}
