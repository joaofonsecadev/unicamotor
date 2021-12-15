// Copyright João Fonseca, All Rights Reserved.

#include "Renderer.h"
#include "Core/UnicaPch.h"

void Renderer::Init(UnicaEngine* const EngineInstance)
{
    EngineCoreComponent::Init(EngineInstance);
    
    Logger::Log(Log, "Initiating renderer...");
}
