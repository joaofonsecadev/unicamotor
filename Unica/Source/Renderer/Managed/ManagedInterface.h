#pragma once

#include <memory>
#include "Renderer/RenderInterface.h"
#include "Renderer/RenderWindow.h"

class ManagedInterface : public RenderInterface
{
public:
    void Init() override;
    void Tick() override;
    void Shutdown() override;

private:
    std::unique_ptr<RenderWindow> m_SdlRenderWindow = std::make_unique<RenderWindow>();

    SDL_Surface* m_SdlWindowSurface = nullptr;
};
