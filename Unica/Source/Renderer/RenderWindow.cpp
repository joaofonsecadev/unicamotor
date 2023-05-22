// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "RenderWindow.h"

#include "UnicaInstance.h"
#include "UnicaMinimal.h"
#include "Log/Logger.h"
#include "UnicaSettings.h"

RenderWindow::RenderWindow()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        UNICA_LOG(spdlog::level::critical, "Couldn't initialize SDL");
    }

    m_SdlWindow = SDL_CreateWindow(UnicaSettings::ApplicationName.c_str(), UnicaSettings::WindowWidth, UnicaSettings::WindowHeight, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    if (m_SdlWindow == nullptr)
    {
        UNICA_LOG(spdlog::level::critical, "Couldn't create an SDL window");
    }

    SDL_SetWindowMinimumSize(m_SdlWindow, 260, 144);

    UNICA_LOG_TRACE("SDL window created");
}

void RenderWindow::Tick()
{
    UNICA_PROFILE_FUNCTION
    SDL_Event SdlEvent;
    {
        UNICA_PROFILE_FUNCTION_NAMED("sdl::SDL_PollEvent");
        SDL_PollEvent(&SdlEvent);
    }

    HandleSdlEvent(SdlEvent);
}

void RenderWindow::HandleSdlEvent(const SDL_Event& Event)
{
    UNICA_PROFILE_FUNCTION
    switch (Event.type)
    {
    case SDL_EVENT_WINDOW_RESIZED:
    {
        UNICA_LOG_DEBUG("Window has been resized to {}x{}", Event.window.data1, Event.window.data2);
        m_bWindowResized = true;
        break;
    }    
    case SDL_EVENT_WINDOW_MINIMIZED:
        UNICA_LOG_DEBUG("Window has been minimized");
        m_bWindowIsMinimized = true;
        while (m_bWindowIsMinimized)
        {
            SDL_Event WaitEvent;
            {
                UNICA_PROFILE_FUNCTION_NAMED("sdl::SDL_WaitEvent");
                SDL_WaitEvent(&WaitEvent);
            }
            if (WaitEvent.type == SDL_EVENT_WINDOW_RESTORED)
            {
                UNICA_LOG_TRACE("Window has been restored");
                m_bWindowIsMinimized = false;
            }
        }
        
        break;
    case SDL_EVENT_QUIT:
        UNICA_LOG_DEBUG("Window's close button (X) was pressed");
        UnicaInstance::RequestExit();
        break;
    default:
        break;
    }
}

RenderWindow::~RenderWindow()
{
    UNICA_LOG_TRACE("Destroying SDL window");
    SDL_DestroyWindow(m_SdlWindow);
    SDL_Quit();
}
