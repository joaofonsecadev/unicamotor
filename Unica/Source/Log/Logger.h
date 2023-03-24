// 2021-2023 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <string>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>

#include "UnicaMinimal.h"

#define UNICA_LOG(LogLevel, ...) SPDLOG_LOGGER_CALL(Logger::GetCoreLogger(), LogLevel, __VA_ARGS__)
#define UNICA_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(Logger::GetCoreLogger(), __VA_ARGS__)
#define UNICA_LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(Logger::GetCoreLogger(), __VA_ARGS__)
#define UNICA_LOG_INFO(...) SPDLOG_LOGGER_INFO(Logger::GetCoreLogger(), __VA_ARGS__)
#define UNICA_LOG_WARN(...) SPDLOG_LOGGER_WARN(Logger::GetCoreLogger(), __VA_ARGS__)
#define UNICA_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(Logger::GetCoreLogger(), __VA_ARGS__)
#define UNICA_LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(Logger::GetCoreLogger(), __VA_ARGS__)

enum LogLevel : uint8
{
    Log,
    Warning,
    Error,
    Fatal
};

class Logger
{
public:
    static void Init();
    static std::shared_ptr<spdlog::logger> GetCoreLogger() { return m_CoreLogger; }
private:
    static std::shared_ptr<spdlog::logger> m_CoreLogger;
};
