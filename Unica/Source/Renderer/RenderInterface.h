#pragma once

class RenderInterface
{
public:
    virtual void Init() = 0;
    virtual void Shutdown() = 0;
    virtual void Tick() = 0;
    virtual ~RenderInterface() = default;
};
