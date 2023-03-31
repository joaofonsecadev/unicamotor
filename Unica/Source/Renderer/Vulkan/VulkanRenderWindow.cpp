// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "VulkanRenderWindow.h"

#include "UnicaInstance.h"
#include "UnicaMinimal.h"
#include "Log/Logger.h"
#include "UnicaSettings.h"

SdlRenderWindow::SdlRenderWindow()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        UNICA_LOG(spdlog::level::critical, "Couldn't initialize SDL");
    }

    m_SdlWindow = SDL_CreateWindow(UnicaSettings::ApplicationName.c_str(), UnicaSettings::WindowWidth, UnicaSettings::WindowHeight, SDL_WINDOW_VULKAN);
    if (m_SdlWindow == nullptr)
    {
        UNICA_LOG(spdlog::level::critical, "Couldn't create an SDL window");
    }
}

void SdlRenderWindow::Tick()
{
    UNICA_PROFILE_FUNCTION
    SDL_Event SdlEvent;
    {
        UNICA_PROFILE_FUNCTION_NAMED("sdl::SDL_PollEvent");
        SDL_PollEvent(&SdlEvent);
    }
    
    if (SdlEvent.type == SDL_EVENT_QUIT)
    {
        UnicaInstance::RequestExit();
    }
}

SdlRenderWindow::~SdlRenderWindow()
{
    SDL_DestroyWindow(m_SdlWindow);
    SDL_Quit();
}
