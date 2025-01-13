#include "thread.h"

#include "profiling.h"

Unica::Thread::Thread()
{
    UnicaProf_ZoneScoped;
#ifdef WIN32
    m_thread = CreateThread(nullptr, 0, ThreadProc, this, 0, nullptr);
    if (m_thread != nullptr)
    {
        m_started = true;
    }
#endif
}

#ifdef WIN32
DWORD Unica::Thread::ThreadProc(LPVOID lp_parameter)
{
    UnicaProf_ZoneScoped;
    Thread* thread = static_cast<Unica::Thread*>(lp_parameter);
    thread->Main();
    thread->m_finished = true;
    return 0;
}
#endif

Unica::Thread::~Thread()
{
    UnicaProf_ZoneScoped;
    if (m_thread == nullptr)
    {
        return;
    }

#ifdef WIN32
    CloseHandle(m_thread);
#endif
}

bool Unica::Thread::Join(const uint64_t timeout_ms) const
{
    UnicaProf_ZoneScoped;
    if (m_thread == nullptr || m_started == false)
    {
        return true;
    }

    if (!m_finished)
    {
        return false;
    }

#ifdef WIN32
    uint64_t wait_result = WaitForSingleObject(m_thread, timeout_ms);
    return wait_result != WAIT_TIMEOUT;
#else
    return true;
#endif

}
