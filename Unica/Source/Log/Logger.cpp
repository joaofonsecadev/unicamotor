// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#include "Logger.h"

#include <chrono>

#include "fmt/printf.h"
#include "fmt/color.h"
#include "fmt/chrono.h"

#include "UnicaInstance.h"

void Logger::Log(LogLevel LogLevel, const std::string& LogCategory, const std::string& LogText)
{
    std::string LogLevelInfoName;
    fmt::text_style LogLevelTextStyle = fg(fmt::color::white);
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

    std::chrono::time_point CurrentTimestamp = std::chrono::system_clock::now();
    const std::string CurrentTimeStampString = fmt::format("{0:%FT%T}", CurrentTimestamp);

    fmt::print(LogLevelTextStyle, "[{}] {}[{}] {}\n", CurrentTimeStampString, LogLevelInfoName, LogCategory, LogText);

    if (LogLevel == Fatal)
    {
        UnicaInstance::RequestExit();
    }
}

