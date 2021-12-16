// Copyright João Fonseca, All Rights Reserved.

#pragma once

#include "Engine/EngineComponent.h"

class UnicaEngine;

class EventHandler : public EngineComponent
{
public:
    void Tick() override;
};
