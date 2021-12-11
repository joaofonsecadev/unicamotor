// Copyright João Fonseca, All Rights Reserved.

#pragma once

class UnicaEngine;

class EngineCoreComponent
{
public:
    virtual ~EngineCoreComponent() { }
    virtual void Init(UnicaEngine* EngineInstance) { m_EngineInstance = EngineInstance; }
    virtual void Tick() { }

    UnicaEngine* GetEngineInstance() const { return m_EngineInstance; }

private:
    UnicaEngine* m_EngineInstance = nullptr;
};
