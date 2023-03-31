// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <tracy/Tracy.hpp>

#include "Log/Logger.h"

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#ifdef NDEBUG
#define UNICA_SHIPPING 1
#else
#define UNICA_SHIPPING 0
#endif

#define UNICA_PROFILE_FUNCTION ZoneScoped;
#define UNICA_PROFILE_FUNCTION_NAMED(x) ZoneScopedN(x)
#define UNICA_PROFILE_FRAME_START(x) FrameMarkStart(x)
#define UNICA_PROFILE_FRAME_END(x) FrameMarkEnd(x)
#define UNICA_PROFILE_FRAME(x) FrameMarkNamed(x)

#define UNICA_LOG(LogLevel, ...) SPDLOG_LOGGER_CALL(Logger::GetCoreLogger(), LogLevel, __VA_ARGS__);if(LogLevel==spdlog::level::critical)throw
#define UNICA_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(Logger::GetCoreLogger(), __VA_ARGS__)
#define UNICA_LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(Logger::GetCoreLogger(), __VA_ARGS__)
#define UNICA_LOG_INFO(...) SPDLOG_LOGGER_INFO(Logger::GetCoreLogger(), __VA_ARGS__)
#define UNICA_LOG_WARN(...) SPDLOG_LOGGER_WARN(Logger::GetCoreLogger(), __VA_ARGS__)
#define UNICA_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(Logger::GetCoreLogger(), __VA_ARGS__)
#define UNICA_LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(Logger::GetCoreLogger(), __VA_ARGS__);throw
