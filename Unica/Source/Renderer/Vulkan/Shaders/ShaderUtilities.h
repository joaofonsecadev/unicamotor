#pragma once

#include <filesystem>
#include <shaderc/shaderc.h>

class ShaderUtilities
{
public:
    static void CompileShaders();

private:
    static shaderc_shader_kind DeduceShaderKind(const std::filesystem::path& GlslShaderFile);
    
    static bool IsShaderCompiled(const std::filesystem::path& GlslShaderFile);
    static bool IsShaderPreProcessed(const std::filesystem::path& GlslShaderFile);
    static bool CheckPreProcessedShadersEqual(const std::filesystem::path& GlslShaderFile, const std::string PreProcessedShader);
};
