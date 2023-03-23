// 2021-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "Logger.h"

#include <chrono>
#include <memory>

#include <fmt/color.h>
#include <fmt/chrono.h>

#include "UnicaInstance.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Logger::m_CoreLogger;
void Logger::Init()
{
    auto StdoutLogSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    m_CoreLogger = std::make_shared<spdlog::logger>("UNICA", StdoutLogSink);
    m_CoreLogger->set_level(spdlog::level::trace);
    m_CoreLogger->set_pattern("[%T.%e] %-37!! %^%5l%$: %v");
}
