#include <tracy/Tracy.hpp>
#include "unicamotor.h"
#include "timer/timer_subsystem.h"
#include "spdlog/spdlog.h"

bool Unicamotor::m_requested_exit = false;

Unicamotor::Unicamotor()
{
    InitializeSubsystems();
}

void Unicamotor::Tick()
{
    ZoneScoped;
    m_timer_subsystem->MarkStartWork();

    for (std::unique_ptr<Subsystem>& subsystem : m_subsystems_vector)
    {
        if (!subsystem->ShouldTick())
        {
            continue;
        }

        subsystem->Tick();
    }

    m_timer_subsystem->MarkEndWork();
    m_timer_subsystem->PossibleSleep();
}

void Unicamotor::InitializeSubsystems()
{
    m_subsystems_map.clear();
    m_subsystems_vector.clear();

    m_subsystems_vector.push_back(std::make_unique<TimerSubsystem>(this));
    m_timer_subsystem = static_cast<TimerSubsystem*>(m_subsystems_vector.at(0).get());

    for (std::unique_ptr<Subsystem>& subsystem : m_subsystems_vector)
    {
        SPDLOG_INFO("Initializing {}", subsystem->GetSubsystemName());

        if (!subsystem->Init())
        {
            SPDLOG_ERROR("Failed to initialize {}", subsystem->GetSubsystemName());
            continue;
        }

        m_subsystems_map.insert(std::pair<std::string, Subsystem*>(subsystem->GetSubsystemName(), subsystem.get()));
    }
}
