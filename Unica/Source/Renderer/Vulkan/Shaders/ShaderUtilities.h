#pragma once

#include <filesystem>
#include <shaderc/shaderc.h>

class ShaderUtilities
{
public:
    static void CompileShaders();

private:
    static shaderc_shader_kind DeduceShaderKind(const std::filesystem::path& GlslShaderFile);
};
