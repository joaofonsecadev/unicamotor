// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <memory>

#include "Subsystem/SubsystemManager.h"

class UnicaInstance
{
public:
    void Init();
    void Tick();
    void Shutdown();

    static bool HasRequestedExit() { return m_bHasRequestedExit; }
    static void RequestExit() { m_bHasRequestedExit = true; }

private:
    // Temporary way oh handling CRTL+C
    static void SignalCallbackHandler(int signum);

    std::unique_ptr<SubsystemManager> m_SubsystemManager;
    static bool m_bHasRequestedExit;
};
