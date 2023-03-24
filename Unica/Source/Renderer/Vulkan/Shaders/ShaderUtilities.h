// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <filesystem>
#include <shaderc/shaderc.h>

#include "UnicaMinimal.h"

class ShaderUtilities
{
public:
    static void CompileShaders();
    static std::vector<char> LoadShader(const std::string& FileLocation);

private:
    static bool WriteCompiledSpirvFile(const std::vector<uint32>& ShaderBinary, const std::string& AbsoluteFileDestination);
    static shaderc_shader_kind DeduceShaderKind(const std::filesystem::path& GlslShaderFile);
};
