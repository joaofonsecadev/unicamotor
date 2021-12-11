// Copyright João Fonseca, All Rights Reserved.

#include "EventHandler.h"

#include "SDL.h"

#include "Engine/UnicaEngine.h"

void EventHandler::Tick()
{
    SDL_Event Event;
    SDL_PollEvent(&Event);
    UnicaEngine* EngineInstance = GetEngineInstance();

    switch (Event.type)
    {
    case SDL_QUIT:
        EngineInstance->RequestExit();
        break;

    default:
        break;
    }
}
