// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

class Logger
{
public:
    static void Init();
    static std::shared_ptr<spdlog::logger> GetCoreLogger() { return m_CoreLogger; }
private:
    static std::shared_ptr<spdlog::logger> m_CoreLogger;
};
