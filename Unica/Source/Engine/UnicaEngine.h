// Copyright João Fonseca, All Rights Reserved.

#pragma once

#include "Graphics/Renderer.h"
#include "Input/EventHandler.h"

class UnicaEngine
{
public:
    UnicaEngine() { }
    void Init();
    void Tick();
    void Exit();

    void RequestExit() { m_bRequestedExit = true; }
    bool GetRequestedExit() const { return m_bRequestedExit; }

private:
    bool m_bRequestedExit = false;

    Renderer m_Renderer = Renderer();
    EventHandler m_EventHandler = EventHandler();
};
