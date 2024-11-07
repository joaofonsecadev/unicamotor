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
    void FindAndCompileShaders();
    static void CallbackFramebufferResize(GLFWwindow* window, int width, int height);

    GLFWwindow* m_window = nullptr;
    int32_t m_shader_program = 0;

    uint32_t m_vertex_array_object = 0;
    uint32_t m_vertex_buffer_object = 0;
};
