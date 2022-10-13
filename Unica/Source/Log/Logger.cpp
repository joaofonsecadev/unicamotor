// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "Logger.h"

#include <chrono>

#include "fmt/color.h"
#include "fmt/chrono.h"

#include "UnicaInstance.h"

void Logger::Log(LogLevel LogLevel, const std::string& LogCategory, const std::string& LogText)
{
    std::string LogLevelInfoName;
    fmt::text_style LogLevelTextStyle = fg(fmt::color::light_gray);
    switch (LogLevel)
    {
        case ::Log:
            break;
        case Warning:
            LogLevelInfoName = "[WARNG] ";
            LogLevelTextStyle = fg(fmt::color::yellow);
            break;
        case Error:
            LogLevelInfoName = "[ERROR] ";
            LogLevelTextStyle = fg(fmt::color::red);
            break;
        case Fatal:
            LogLevelInfoName = "[FATAL] ";
            LogLevelTextStyle = fg(fmt::color::dark_magenta) | fmt::emphasis::bold;
            break;
    }

    const std::string FinalLogString = fmt::format("[{}] {}[{}] {}\n", GetLogTimestamp(), LogLevelInfoName, LogCategory, LogText);
    fmt::print(LogLevelTextStyle, FinalLogString);

    if (LogLevel == Fatal)
    {
        UnicaInstance::RequestExit();
    }
}

std::string Logger::GetLogTimestamp()
{
    const std::chrono::time_point CurrentTimestamp = std::chrono::system_clock::now();
    const std::chrono::duration TimeSinceEpoch = CurrentTimestamp.time_since_epoch();

    const uint64 SecSinceEpochInMillis = std::chrono::duration_cast<std::chrono::seconds>(TimeSinceEpoch).count() * 1000;
    const uint64 MillisSinceEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(TimeSinceEpoch).count();

    return fmt::format("{0:%FT%T}.{1:03d}", CurrentTimestamp, MillisSinceEpoch - SecSinceEpochInMillis);
}

