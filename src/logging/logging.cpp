#include "logging.h"

#include <cstdio>

#include "core/profiling.h"

int32_t LoggingThread::Main()
{
    for (int32_t i = 0; i < 10; i++)
    {
        printf("LoggingThread::Main\n");
    }

    return 0;
}

Logging::~Logging()
{
    while(!m_logging_thread.Join());
}
