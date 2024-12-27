#pragma once

#include "tracy/Tracy.hpp"

#define UnicaProf_ZoneScoped ZoneScoped;
#define UnicaProf_FrameMarkStart(name) FrameMarkStart(name);
#define UnicaProf_FrameMarkEnd(name) FrameMarkEnd(name);
#define UnicaProf_FrameMarkNamed(name) FrameMarkNamed(name);
