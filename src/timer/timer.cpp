#include <tracy/Tracy.hpp>
#include "timer.h"

#include <toml++/toml.hpp>

#include "core/arguments.h"
#include "core/directories.h"
#include "spdlog/spdlog.h"

TimerSubsystem::TimerSubsystem(Unicamotor *engine) : Subsystem(engine)
{
    std::string* frame_time_string = CommandLineParser::Get().GetArgumentValue("--target-frame-time-ms");
    if (frame_time_string == nullptr || frame_time_string->empty())
    {
        toml::parse_result engine_config = toml::parse_file(DirectoriesHelper::EngineDefaultConfigDirectory().c_str());
        m_target_frame_time = std::chrono::nanoseconds(engine_config["CORE"]["TARGET_FRAME_TIME_NS"].value_or(0));
        return;
    }

    float frame_time_argument_ms_float = 16.666666f;
    try
    {
        frame_time_argument_ms_float = std::stof(*frame_time_string);
    }
    catch (const std::invalid_argument& exception)
    {
        SPDLOG_ERROR("Argument '{}' for --target-frame-time-ms is invalid, defaulting to 16ms frame time", *frame_time_string);
    }
    catch (const std::out_of_range& exception)
    {
        SPDLOG_ERROR("Argument '{}' for --target-frame-time-ms does not fit into a float, defaulting to 16ms frame time", *frame_time_string);
    }

    const uint32_t frame_time_nanoseconds = static_cast<uint32_t>(frame_time_argument_ms_float * 1'000'000);
    m_target_frame_time = std::chrono::nanoseconds(frame_time_nanoseconds);
}

void TimerSubsystem::Tick()
{
    ZoneScoped;
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();

    m_previous_frame_time = current_time - m_previous_frame_start_time;
    m_previous_frame_start_time = current_time;
}

void TimerSubsystem::MarkStartWork()
{
    ZoneScoped;
    m_current_frame_start_work_time = std::chrono::steady_clock::now();
}

void TimerSubsystem::MarkEndWork()
{
    ZoneScoped;
    m_current_frame_work_time = std::chrono::steady_clock::now() - m_current_frame_start_work_time;
}

void TimerSubsystem::PossibleSleep()
{
    ZoneScoped;
    while (std::chrono::steady_clock::now() - m_current_frame_start_work_time < m_target_frame_time) { }
}
