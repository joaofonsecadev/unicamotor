// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include "Subsystem/SubsystemManager.h"

class UnicaInstance
{
public:
    void Init();
    void Tick();
    void Shutdown();

    static bool HasRequestedExit() { return m_bHasRequestedExit; }
    static void RequestExit() { m_bHasRequestedExit = true; }
    
    static std::filesystem::path GetUnicaRootDirectory() { return m_UnicaRootDirectory; }
    static void SetUnicaRootDirectory(char* SystemStyledExecutableDirectory);

private:
    void TickLogic();
    
    std::unique_ptr<SubsystemManager> m_SubsystemManager;
    
    static bool m_bHasRequestedExit;
    static std::filesystem::path m_UnicaRootDirectory;
};
