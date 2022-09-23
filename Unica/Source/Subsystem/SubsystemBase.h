// Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

class SubsystemBase
{
public:
    virtual void Init() = 0;
    virtual void Shutdown() = 0;
    virtual void Tick() { }

    virtual bool IsTicking() { return false; }
};
