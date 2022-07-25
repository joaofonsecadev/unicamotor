// Copyright joaofonseca.dev, All Rights Reserved.

#include "SubsystemManager.h"

#include <memory>
#include "Timer/TimeManager.h"

std::vector<std::unique_ptr<SubsystemBase>> SubsystemManager::m_SubsystemCollection;

void SubsystemManager::Init()
{
    InitializeSubsystem(new TimeManager);
}

void SubsystemManager::Shutdown()
{
    while (!m_SubsystemCollection.empty())
    {
        SubsystemBase* const Subsystem = m_SubsystemCollection.back().get();
        Subsystem->Shutdown();

        m_SubsystemCollection.pop_back();
    }
}

void SubsystemManager::InitializeSubsystem(SubsystemBase* const Subsystem)
{
    Subsystem->Init();
    m_SubsystemCollection.push_back(std::unique_ptr<SubsystemBase>(Subsystem));
}
