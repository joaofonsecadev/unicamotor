#include "opengl.h"

#include "spdlog/spdlog.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "core/unicamotor.h"
#include "core/directories.h"
#include "tracy/Tracy.hpp"

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

    FindAndCompileShaders();
    float vertices[] = { -0.5f, -0.5f, 0.f,
                         0.5f, -0.5f, 0.f,
                         0.f, 0.5f, 0.f };

    glGenVertexArrays(1, &m_vertex_array_object);
    glGenBuffers(1, &m_vertex_buffer_object);
    glBindVertexArray(m_vertex_array_object);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return true;
}

void RendererOpengl::Tick()
{
    glClearColor(0.33f, 0.33f, 0.39f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_shader_program);
    glBindVertexArray(m_vertex_array_object);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
    if (glfwWindowShouldClose(m_window))
    {
        Unicamotor::RequestExit();
    }
}

void RendererOpengl::FindAndCompileShaders()
{
    ZoneScoped;
    std::vector<std::filesystem::path> shader_files = DirectoriesHelper::GetAllFilesInDirectory(DirectoriesHelper::EngineShadersDirectory(), { ".vert", ".frag" });
    SPDLOG_INFO("Compiling {} shaders", shader_files.size());

    m_shader_program = glCreateProgram();
    for (const std::filesystem::path& shader_file : shader_files)
    {
        const std::string shader_source_string = DirectoriesHelper::ReadFileAsString(shader_file);
        const char* shader_source = shader_source_string.c_str();

        uint32_t shader_type = 0;
        const std::string& shader_extension = shader_file.extension().string();
        if (shader_extension == ".frag")
        {
            shader_type = glCreateShader(GL_FRAGMENT_SHADER);
        }
        else if (shader_extension == ".vert")
        {
            shader_type = glCreateShader(GL_VERTEX_SHADER);
        }

        glShaderSource(shader_type, 1, &shader_source, nullptr);
        glCompileShader(shader_type);

        char shader_compilation_log[512];
        int32_t shader_compilation_success = 0;
        glGetShaderiv(shader_type, GL_COMPILE_STATUS, &shader_compilation_success);
        if (!shader_compilation_success)
        {
            glGetShaderInfoLog(shader_type, sizeof(shader_compilation_log), nullptr, shader_compilation_log);
            SPDLOG_ERROR("Failed shader compilation for {}: {}", shader_file.string(), shader_compilation_log);
            continue;
        }

        glAttachShader(m_shader_program, shader_type);
        glLinkProgram(m_shader_program);
        glGetProgramiv(m_shader_program, GL_LINK_STATUS, &shader_compilation_success);
        if (!shader_compilation_success)
        {
            glGetProgramInfoLog(m_shader_program, sizeof(shader_compilation_log), nullptr, shader_compilation_log);
            SPDLOG_ERROR("Failed shader linkage for {}: {}", shader_file.string(), shader_compilation_log);
            continue;
        }

        glDeleteShader(shader_type);
    }
}

void RendererOpengl::CallbackFramebufferResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

RendererOpengl::~RendererOpengl()
{
    glDeleteVertexArrays(1, &m_vertex_array_object);
    glDeleteBuffers(1, &m_vertex_buffer_object);
    glDeleteProgram(m_shader_program);
    glfwTerminate();
}
