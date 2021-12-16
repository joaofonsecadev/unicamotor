// Copyright João Fonseca, All Rights Reserved.

#pragma once

#include "Engine/EngineCoreComponent.h"

class UnicaEngine;

class EventHandler : public EngineCoreComponent
{
public:
    void Tick() override;
};
