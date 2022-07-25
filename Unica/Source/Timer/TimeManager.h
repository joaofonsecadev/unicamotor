// Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include "Subsystem/SubsystemBase.h"

class TimeManager final : public SubsystemBase
{
private:
    void Init() override;
    void Shutdown() override;
};