#include "ManagedInterface.h"

#include "UnicaMinimal.h"

void ManagedInterface::Init()
{
    m_SdlWindowSurface = SDL_GetWindowSurface(m_SdlRenderWindow->GetSdlWindow());
    if (m_SdlWindowSurface == nullptr)
    {
        UNICA_LOG_CRITICAL("Failed to create SDL_WindowSurface");
    }
}

void ManagedInterface::Tick()
{
    UNICA_PROFILE_FUNCTION
    m_SdlRenderWindow->Tick();
}

void ManagedInterface::Shutdown()
{
}
