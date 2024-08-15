#include "opengl.h"

#include "spdlog/spdlog.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "core/unicamotor.h"

RendererOpengl::RendererOpengl(Unicamotor* engine) : RendererSubsystem(engine)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_window = glfwCreateWindow(1280, 720, "Unicamotor", nullptr, nullptr);
    if (m_window == nullptr)
    {
        SPDLOG_CRITICAL("OpenGL failed critically when creating window");
        Unicamotor::RequestExit();
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        SPDLOG_CRITICAL("GLAD initialization failed critically");
        Unicamotor::RequestExit();
        return;
    }

    glViewport(0, 0, 1280, 720);
    glfwSetFramebufferSizeCallback(m_window, CallbackFramebufferResize);
}

bool RendererOpengl::Init()
{
    if (m_window == nullptr)
    {
        return false;
    }

    return true;
}

void RendererOpengl::Tick()
{
    glfwPollEvents();

    glClearColor(0.33f, 0.33f, 0.39f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(m_window);
    if (glfwWindowShouldClose(m_window))
    {
        Unicamotor::RequestExit();
    }
}

void RendererOpengl::CallbackFramebufferResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

RendererOpengl::~RendererOpengl()
{
    glfwTerminate();
}
