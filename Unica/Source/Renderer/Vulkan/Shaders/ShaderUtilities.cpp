#include "ShaderUtilities.h"

#include <string>
#include <vector>
#include <shaderc/shaderc.hpp>

#include "UnicaFileUtilities.h"
#include "Log/Logger.h"

void ShaderUtilities::CompileShaders()
{
    const std::vector<std::string> ShaderFileExtensionsToCompile = { ".frag", ".vert" };
    const std::vector<std::filesystem::path> GlslShaderFiles = UnicaFileUtilities::GetFilesInPathWithExtension("Shaders", ShaderFileExtensionsToCompile);
    if (GlslShaderFiles.empty())
    {
        UNICA_LOG(Log, __FUNCTION__, "No GLSL shaders to compile");
        return;
    }
	
    shaderc::Compiler Compiler;
    shaderc::CompileOptions CompilerOptions;

    for (const std::filesystem::path& GlslShaderFile : GlslShaderFiles)
    {
        std::string GlslShaderString = UnicaFileUtilities::ReadFileAsString(GlslShaderFile.string());
        if (GlslShaderString.empty())
        {
            UNICA_LOG(Error, __FUNCTION__, std::format("Can't read shader file '{}'", GlslShaderFile.string()));
            continue;
        }
        
        const shaderc_shader_kind ShaderKind = DeduceShaderKind(GlslShaderFile);
        shaderc::PreprocessedSourceCompilationResult PreProcessedShaderResult = Compiler.PreprocessGlsl(GlslShaderString,
            ShaderKind, GlslShaderFile.filename().string().c_str(), CompilerOptions);

        std::string PreProcessedShaderSource(PreProcessedShaderResult.begin(), PreProcessedShaderResult.end());
        shaderc::SpvCompilationResult ShaderSpvResult = Compiler.CompileGlslToSpv(PreProcessedShaderSource, ShaderKind,
            GlslShaderFile.filename().string().c_str(), CompilerOptions);
    }
}

shaderc_shader_kind ShaderUtilities::DeduceShaderKind(const std::filesystem::path& GlslShaderFile)
{
    const std::string GlslShaderFileExtension = GlslShaderFile.extension().string();
    if (GlslShaderFileExtension == ".frag")
    {
        return shaderc_glsl_fragment_shader;
    }
    if (GlslShaderFileExtension == ".vert")
    {
        return shaderc_glsl_vertex_shader;
    }
    return shaderc_glsl_infer_from_source;
    
}
