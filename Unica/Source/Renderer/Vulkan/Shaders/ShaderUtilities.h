// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

#pragma once

#include <filesystem>
#include <shaderc/shaderc.h>

#include "UnicaMinimal.h"

class ShaderUtilities
{
public:
    static std::vector<char> LoadShader(const std::string& FileLocation);
    
};
