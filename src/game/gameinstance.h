#pragma once

#include <memory>
#include "subsystem/subsystem.h"

class GameInstance : public Subsystem
{
public:
    GameInstance();
    ~GameInstance() override = default;

    bool ShouldTick() override { return true; }
    std::string GetSubsystemName() override { return "GameInstance"; }
};
