#pragma once

#include <filesystem>
#include <string>
#include <vector>

class DirectoriesHelper
{
public:
    static void CalculateBaseDirectories(char* system_arg_zero);
    static std::filesystem::path EngineRootDirectory() { return m_engine_root_directory; }
    static std::filesystem::path EngineContentDirectory() { return EngineRootDirectory().append("content"); }
    static std::filesystem::path EngineShadersDirectory() { return EngineContentDirectory().append("shaders"); }

    static std::string ReadFileAsString(const std::filesystem::path& file_path);
    static std::vector<std::filesystem::path> GetAllFilesInDirectory(const std::filesystem::path& directory, const std::vector<std::string>& extensions = std::vector<std::string>());

private:
    static std::filesystem::path m_binary_path;
    static std::filesystem::path m_engine_root_directory;
    static std::filesystem::path m_project_root_directory;
};
