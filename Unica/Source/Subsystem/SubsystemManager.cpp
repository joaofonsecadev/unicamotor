// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "SubsystemManager.h"

#include <memory>
#include <tracy/Tracy.hpp>

#include "fmt/format.h"

#include "UnicaMinimal.h"
#include "Timer/TimeManager.h"
#include "Renderer/RenderManager.h"

std::vector<std::unique_ptr<SubsystemBase>> SubsystemManager::m_SubsystemCollection;
void SubsystemManager::Init()
{
    InitializeSubsystem(new TimeManager);
    InitializeSubsystem(new RenderManager);
}

void SubsystemManager::InitializeSubsystem(SubsystemBase* const Subsystem)
{
    UNICA_LOG(Log, __FUNCTION__, fmt::format("Initializing {}", typeid(*Subsystem).name()));
    Subsystem->Init();
    m_SubsystemCollection.push_back(std::unique_ptr<SubsystemBase>(Subsystem));
}

void SubsystemManager::Shutdown()
{
    while (!m_SubsystemCollection.empty())
    {
        SubsystemBase* const Subsystem = m_SubsystemCollection.back().get();

        UNICA_LOG(Log, __FUNCTION__, fmt::format("Shutting down {}", typeid(*Subsystem).name()));
        Subsystem->Shutdown();

        m_SubsystemCollection.pop_back();
    }
}

void SubsystemManager::TickSubsystems()
{
    for (const std::unique_ptr<SubsystemBase>& Subsystem : m_SubsystemCollection)
    {
        if (!Subsystem->ShouldTick())
        {
            continue;
        }
        Subsystem->Tick();
    }
}
