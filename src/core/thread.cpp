#include "thread.h"

unc::thread::thread()
{
#ifdef WIN32
    m_thread = CreateThread(nullptr, 0, ThreadProc, this, 0, nullptr);
    if (m_thread == nullptr)
    {
        m_is_running = false;
    }
    m_is_running = true;
#endif
}

#ifdef WIN32
DWORD unc::thread::ThreadProc(LPVOID lp_parameter)
{
    thread* thread = static_cast<unc::thread*>(lp_parameter);
    thread->Main();
    thread->m_is_running = false;
    return 0;
}
#endif

unc::thread::~thread()
{
    if (m_thread == nullptr)
    {
        return;
    }

#ifdef WIN32
    CloseHandle(m_thread);
#endif
}

bool unc::thread::Join(const uint64_t timeout_ms) const
{
    if (m_thread == nullptr || m_is_running == false)
    {
        return true;
    }

#ifdef WIN32
    uint64_t wait_result = WaitForSingleObject(m_thread, timeout_ms);
    return wait_result == WAIT_TIMEOUT;
#else
    return true;
#endif

}
