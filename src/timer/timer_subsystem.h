#pragma once

#include "subsystem/subsystem.h"

class TimerSubsystem : public Subsystem
{
public:
    TimerSubsystem() = delete;
    explicit TimerSubsystem(Unicamotor* engine);
    ~TimerSubsystem() override = default;

    bool ShouldTick() override { return true; }
    std::string GetSubsystemName() override { return "TimerSubsystem"; }

    void Tick() override;

    void MarkStartWork();
    void MarkEndWork();
    void PossibleSleep();

private:
    std::chrono::steady_clock::duration m_target_frame_time = std::chrono::nanoseconds(1);

    std::chrono::steady_clock::time_point m_previous_frame_start_time = std::chrono::steady_clock::now();
    std::chrono::steady_clock::duration m_previous_frame_time = std::chrono::nanoseconds(1);

    std::chrono::steady_clock::time_point m_current_frame_start_work_time = std::chrono::steady_clock::now();
    std::chrono::steady_clock::duration m_current_frame_work_time = std::chrono::nanoseconds(1);
};
