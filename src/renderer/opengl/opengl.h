#pragma once

#include "renderer/renderer.h"

struct GLFWwindow;

class RendererOpengl : public RendererSubsystem
{
public:
    RendererOpengl() = delete;
    explicit RendererOpengl(Unicamotor* engine);
    ~RendererOpengl() override;

    bool Init() override;
    void Tick() override;

private:
    static void CallbackFramebufferResize(GLFWwindow* window, int width, int height);

    GLFWwindow* m_window = nullptr;
};
