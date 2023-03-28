// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#include "ShaderUtilities.h"

#include <fstream>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <shaderc/shaderc.hpp>

#include "UnicaFileUtilities.h"
#include "Log/Logger.h"

std::vector<char> ShaderUtilities::LoadShader(const std::string& FileLocation)
{
    const std::string SpvFileName = FileLocation + ".spv";
    std::vector<char> SpvShaderBinary = UnicaFileUtilities::ReadFileAsBinary(SpvFileName);

    if (SpvShaderBinary.empty())
    {
        UNICA_LOG(spdlog::level::err, std::format("Shader '{}' may not be compiled", FileLocation));
    }
    
    return SpvShaderBinary;
}
