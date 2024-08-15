#pragma once

#include "renderer/renderer.h"

class RendererOpengl : public RendererSubsystem
{
public:
    RendererOpengl() = delete;
    explicit RendererOpengl(Unicamotor* engine);
    ~RendererOpengl() override = default;

    void Tick() override;
};
