// Copyright João Fonseca, All Rights Reserved.

#pragma once

class IEngineCoreComponent
{
public:
    virtual ~IEngineCoreComponent() { }
    virtual void Tick() = 0;
};
