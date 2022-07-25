// Copyright joaofonseca.dev, All Rights Reserved.

#include <memory>
#include "SubsystemManager.h"
#include "Timer/TimeManager.h"

void SubsystemManager::Init()
{
    m_SubsystemCollection.push(std::make_unique<TimeManager>());
}

void SubsystemManager::Shutdown()
{
    while (!SubsystemManager::m_SubsystemCollection.empty())
    {
        SubsystemBase* const Subsystem = SubsystemManager::m_SubsystemCollection.top().get();
        Subsystem->Shutdown();

        SubsystemManager::m_SubsystemCollection.pop();
    }
}
