// Copyright joaofonseca.dev, All Rights Reserved.

#include "SubsystemManager.h"

#include <memory>
#include "Log/Logger.h"
#include "Timer/TimeManager.h"
#include "fmt/format.h"

std::vector<std::unique_ptr<SubsystemBase>> SubsystemManager::m_SubsystemCollection;

void SubsystemManager::Init()
{
    InitializeSubsystem(new TimeManager);
}

void SubsystemManager::InitializeSubsystem(SubsystemBase* const Subsystem)
{
    UNICA_LOG(Log, LogCategory::SubsystemManager, fmt::format("Initializing {}", typeid(*Subsystem).name()));
    Subsystem->Init();
    m_SubsystemCollection.push_back(std::unique_ptr<SubsystemBase>(Subsystem));
}

void SubsystemManager::Shutdown()
{
    while (!m_SubsystemCollection.empty())
    {
        SubsystemBase* const Subsystem = m_SubsystemCollection.back().get();

        UNICA_LOG(Log, LogCategory::SubsystemManager, fmt::format("Shutting down {}", typeid(*Subsystem).name()));
        Subsystem->Shutdown();

        m_SubsystemCollection.pop_back();
    }
}
