// Copyright João Fonseca, All Rights Reserved.

#pragma once
#include "Interfaces/EngineCoreComponent.h"

class UnicaEngine;

class EventHandler : IEngineCoreComponent
{
public:
    EventHandler() { }
    void Init(UnicaEngine* const EngineInstance) {  m_EngineInstance = EngineInstance; }
    void Tick() override;

private:
    UnicaEngine* m_EngineInstance = nullptr;
};
