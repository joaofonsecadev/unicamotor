#pragma once
#include "core/thread.h"

class LoggingThread : public Unica::Thread
{
public:
    int32_t Main() override;
};

class Logging
{
public:
    Logging() = default;
    ~Logging();

private:
    LoggingThread m_logging_thread;
};
