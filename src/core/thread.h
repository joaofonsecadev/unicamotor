#pragma once

#include <cstdint>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace unc
{
class thread
{
public:
    thread();
    virtual ~thread();

    bool Join(uint64_t timeout_ms = UINT64_MAX) const;

protected:
    virtual int32_t Main() = 0;

private:
#ifdef WIN32
    static DWORD WINAPI ThreadProc(LPVOID lpParameter);
#endif

    void* m_thread = nullptr;
    bool m_is_running = false;
};
}
