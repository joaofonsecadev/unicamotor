// Copyright João Fonseca, All Rights Reserved.

#pragma once

#include "Window.h"
#include "Engine/EngineComponent.h"

class UnicaEngine;
class Window;

class Renderer : public EngineComponent
{
public:
    Renderer() { }
    void Init(UnicaEngine* EngineInstance) override;

private:
    Window m_Window;
};
