#pragma once

#include "renderer/renderer.h"

struct GLFWwindow;

class RendererVulkan : public RendererSubsystem
{
public:
    RendererVulkan() = delete;
    explicit RendererVulkan(Unicamotor* engine);
    ~RendererVulkan() override;

    bool Init() override;
    void Tick() override;

private:
    bool CreateWindow();

    // GLFW
    GLFWwindow* m_window = nullptr;
};