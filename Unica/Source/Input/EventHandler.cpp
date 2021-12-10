// Copyright João Fonseca, All Rights Reserved.

#include "EventHandler.h"
#include "SDL.h"
#include "Engine/UnicaEngine.h"
#include "Log/Log.h"

void EventHandler::Tick()
{
    SDL_Event Event;
    SDL_PollEvent(&Event);

    switch (Event.type)
    {
    case SDL_QUIT:
        m_EngineInstance->RequestExit();
        break;

    default:
        break;
    }
}
