#include "system.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "profiling.h"

bool unc::SystemUtilities::SetCurrentThreadAffinity()
{
    UnicaProf_ZoneScoped;
#ifdef WIN32

    uint64_t process_affinity_mask;
    uint64_t system_affinity_mask;
    if (GetProcessAffinityMask(GetCurrentProcess(), &process_affinity_mask, &system_affinity_mask) == 0)
    {
        auto ccc = GetLastError();
        return false;
    }

    uint64_t core_mask = 1;
    while (core_mask & process_affinity_mask == 0 && core_mask != 0)
    {
        core_mask <<= 1;
    }

    if (core_mask == 0)
    {
        return false;
    }

    uint64_t set_affinity_result = SetThreadAffinityMask(GetCurrentThread(), core_mask);
    return set_affinity_result != 0;
#else
    return true;
#endif
}
