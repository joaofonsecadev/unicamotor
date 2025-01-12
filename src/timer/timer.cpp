#include "timer.h"

Timer::Timer()
{
#ifdef WIN32
    QueryPerformanceFrequency(&m_frequency);
    QueryPerformanceCounter(&m_start_counter);

    m_nanos_per_tick = 1'000'000'000ULL / m_frequency.QuadPart;
#endif
}

uint64_t Timer::GetCurrentTimeInNanoseconds() const
{
#ifdef WIN32
    //@TODO possibly revisit due to loss of precision
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    return counter.QuadPart * m_nanos_per_tick;
#else
    return 0;
#endif
}
