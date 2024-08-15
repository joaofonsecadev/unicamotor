#include <tracy/Tracy.hpp>
#include "unicamotor.h"
#include "timer/timer.h"
#include "spdlog/spdlog.h"
#include "renderer/opengl/opengl.h"
#include "arguments.h"

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

    ResolveGraphicsApi(m_subsystems_vector);

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

void Unicamotor::ResolveGraphicsApi(std::vector<std::unique_ptr<Subsystem>>& subsystems)
{
    std::string* graphics_api = CommandLineParser::Get().GetArgumentValue("--renderer-graphics-api");
    if (graphics_api == nullptr || graphics_api->empty())
    {
        m_subsystems_vector.push_back(std::make_unique<RendererOpengl>(this));
        SPDLOG_INFO("Selected the default graphics API: OpenGL");
        return;
    }

    if (*graphics_api == "opengl")
    {
        m_subsystems_vector.push_back(std::make_unique<RendererOpengl>(this));
        SPDLOG_INFO("Selected graphics API: OpenGL");
        return;
    }
    else
    {
        m_subsystems_vector.push_back(std::make_unique<RendererOpengl>(this));
        SPDLOG_INFO("Failed to resolve graphics api argument '{}', defaulting to OpenGL", *graphics_api);
        return;
    }
}
