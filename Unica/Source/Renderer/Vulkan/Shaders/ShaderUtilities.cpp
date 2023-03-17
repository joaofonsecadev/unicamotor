#include "ShaderUtilities.h"

#include <fstream>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <shaderc/shaderc.hpp>

#include "UnicaFileUtilities.h"
#include "Log/Logger.h"

void ShaderUtilities::CompileShaders()
{
    const std::vector<std::string> ShaderFileExtensionsToCompile = { ".frag", ".vert" };
    const std::vector<std::filesystem::path> GlslShaderFiles = UnicaFileUtilities::GetFilesInPathWithExtension("Engine:Shaders", ShaderFileExtensionsToCompile);
    if (GlslShaderFiles.empty())
    {
        UNICA_LOG(Log, __FUNCTION__, "No GLSL shaders to compile");
        return;
    }

    const std::chrono::time_point ShaderCompilationStartTime = std::chrono::high_resolution_clock::now();
    UNICA_LOG(Log, __FUNCTION__, "Starting shader compilation");
	
    shaderc::Compiler Compiler;
    shaderc::CompileOptions CompilerOptions;
    for (const std::filesystem::path& GlslShaderFile : GlslShaderFiles)
    {
        std::string GlslShaderString = UnicaFileUtilities::ReadFileAsString(GlslShaderFile.string());
        if (GlslShaderString.empty())
        {
            UNICA_LOG(Error, __FUNCTION__, fmt::format("Can't read shader file '{}'", GlslShaderFile.string()));
            continue;
        }
        
        const shaderc_shader_kind ShaderKind = DeduceShaderKind(GlslShaderFile);
        shaderc::PreprocessedSourceCompilationResult PreProcessedShaderResult = Compiler.PreprocessGlsl(GlslShaderString,
            ShaderKind, GlslShaderFile.filename().string().c_str(), CompilerOptions);

        const std::vector<char> PreProcessedShaderBinary(PreProcessedShaderResult.begin(), PreProcessedShaderResult.end());
        std::string PreProcessedShaderSource(PreProcessedShaderResult.begin(), PreProcessedShaderResult.end());
        shaderc::SpvCompilationResult ShaderSpvResult = Compiler.CompileGlslToSpv(PreProcessedShaderSource, ShaderKind,
            GlslShaderFile.filename().string().c_str(), CompilerOptions);

        const std::vector<uint32> ShaderSpvResultBinary(ShaderSpvResult.begin(), ShaderSpvResult.end());
        WriteCompiledSpirvFile(ShaderSpvResultBinary, GlslShaderFile.string().append(".spv"));
    }

    const std::chrono::time_point ShaderCompilationEndTime = std::chrono::high_resolution_clock::now();
    const auto ShaderCompilationTimeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(ShaderCompilationEndTime - ShaderCompilationStartTime);
    UNICA_LOG(Log, __FUNCTION__, fmt::format("Shader compilation completed in {} milliseconds", ShaderCompilationTimeTaken.count()));
}

std::vector<char> ShaderUtilities::LoadShader(const std::string& FileLocation)
{
    const std::string SpvFileName = FileLocation + ".spv";
    std::vector<char> SpvShaderBinary = UnicaFileUtilities::ReadFileAsBinary(SpvFileName);

    if (SpvShaderBinary.empty())
    {
        UNICA_LOG(Error, __FUNCTION__, std::format("Shader '{}' may not be compiled", FileLocation));
    }
    
    return SpvShaderBinary;
}

bool ShaderUtilities::WriteCompiledSpirvFile(const std::vector<uint32>& ShaderBinary, const std::string& AbsoluteFileDestination)
{
    const std::filesystem::path FileDirectory(AbsoluteFileDestination);
    std::ofstream OutputFile(FileDirectory.string(), std::ios::trunc | std::ios::binary);

    if (!OutputFile)
    {
        UNICA_LOG(Error, __FUNCTION__, fmt::format("Can't open file '{}' for writting", FileDirectory.string()));
        return false;
    }

    OutputFile.write(reinterpret_cast<const char*>(ShaderBinary.data()), ShaderBinary.size() * sizeof(uint32));
    return true;
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
