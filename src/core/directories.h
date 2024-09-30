#pragma once

#include <filesystem>

class DirectoriesHelper
{
public:
    static void CalculateBaseDirectories(char* system_arg_zero);
    static std::filesystem::path EngineRootDirectory() { return m_engine_root_directory; }
    static std::filesystem::path EngineContentDirectory() { return EngineRootDirectory().append("content"); }
    static std::filesystem::path EngineShadersDirectory() { return EngineContentDirectory().append("shaders"); }

private:
    static std::filesystem::path m_binary_path;
    static std::filesystem::path m_engine_root_directory;
    static std::filesystem::path m_project_root_directory;
};
