// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#include "Log/Logger.h"
#include "UnicaSettings.h"
#include "tracy/Tracy.hpp"

#ifdef NDEBUG
#define UNICA_SHIPPING = 1
#else
#define UNICA_SHIPPING = 0
#endif

#define UNICA_PROFILE_FUNCTION ZoneScoped;
#define UNICA_PROFILE_FUNCTION_NAMED(x) ZoneScopedN(x);
#define UNICA_PROFILE_FRAME_START(x) FrameMarkStart(x);
#define UNICA_PROFILE_FRAME_END(x) FrameMarkEnd(x);
#define UNICA_PROFILE_FRAME(x) FrameMarkNamed(x);
