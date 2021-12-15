// Copyright João Fonseca, All Rights Reserved.

#pragma once

#include "Engine/EngineCoreComponent.h"

class UnicaEngine;

class Renderer : public EngineCoreComponent
{
public:
    Renderer() { }
    void Init(UnicaEngine* EngineInstance) override;

};
