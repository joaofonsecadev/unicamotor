#pragma once

#include <cstdint>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "core/profiling.h"

class Timer
{
public:
    Timer();
    ~Timer() = default;

    void SetMinimumFrameTime(const float min_frame_time_ms) { m_minimum_frame_time = static_cast<uint64_t>(min_frame_time_ms * 1'000'000); }

    void MarkStartWork() { UnicaProf_ZoneScoped; m_current_frame_start_work_time_point = GetCurrentTimeInNanoseconds(); }
    void MarkEndWork() { UnicaProf_ZoneScoped; m_current_frame_work_time = GetCurrentTimeInNanoseconds() - m_current_frame_start_work_time_point; }
    void PossibleSleep() const { UnicaProf_ZoneScoped; if (m_minimum_frame_time == 0) return; while (GetCurrentTimeInNanoseconds() - m_current_frame_start_work_time_point < m_minimum_frame_time); }

    uint64_t GetCurrentTimeInNanoseconds() const;
private:
#ifdef WIN32
    LARGE_INTEGER m_frequency = { };
    LARGE_INTEGER m_start_counter = { };
#else
#endif

    uint64_t m_nanos_per_tick = 0;
    uint64_t m_minimum_frame_time = 16.666666f * 1'000'000;

    uint64_t m_current_frame_start_work_time_point = 0;
    uint64_t m_current_frame_work_time = 0;

};
