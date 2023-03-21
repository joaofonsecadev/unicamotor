// 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

typedef signed char         int8;
typedef short               int16;
typedef int                 int32;
typedef long long           int64;

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;

#include "Log/Logger.h"
#include "UnicaSettings.h"
#include "tracy/Tracy.hpp"

#ifdef NDEBUG
#define UNICA_SHIPPING
#endif

#define UNICA_PROFILE_FUNCTION ZoneScoped;
#define UNICA_PROFILE_FUNCTION_NAMED(x) ZoneScopedN(x);
#define UNICA_PROFILE_FRAME_START(x) FrameMarkStart(x);
#define UNICA_PROFILE_FRAME_END(x) FrameMarkEnd(x);
#define UNICA_PROFILE_FRAME(x) FrameMarkNamed(x);
