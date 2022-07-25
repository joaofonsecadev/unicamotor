// Copyright joaofonseca.dev, All Rights Reserved.

#include <memory>
#include "SubsystemManager.h"
#include "Timer/TimeManager.h"

std::stack<std::unique_ptr<SubsystemBase>> SubsystemManager::m_SubsystemCollection;

void SubsystemManager::Init()
{
    InitializeSubsystem(new TimeManager);
}

void SubsystemManager::Shutdown()
{
    while (!m_SubsystemCollection.empty())
    {
        SubsystemBase* const Subsystem = m_SubsystemCollection.top().get();
        Subsystem->Shutdown();

        m_SubsystemCollection.pop();
    }
}

void SubsystemManager::InitializeSubsystem(SubsystemBase* const Subsystem)
{
    Subsystem->Init();
    m_SubsystemCollection.push(std::unique_ptr<SubsystemBase>(Subsystem));
}
