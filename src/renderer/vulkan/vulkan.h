#pragma once

#include "renderer/renderer.h"

class RendererVulkan : public RendererSubsystem
{
public:
    RendererVulkan() = delete;
    explicit RendererVulkan(Unicamotor* engine);
    ~RendererVulkan() override;

    bool Init() override;
    void Tick() override;
};