#include "directories.h"

#include <fstream>
#include "spdlog/spdlog.h"
#include "unicamotor.h"

std::filesystem::path DirectoriesHelper::m_binary_path;
std::filesystem::path DirectoriesHelper::m_engine_root_directory;
std::filesystem::path DirectoriesHelper::m_project_root_directory;

void DirectoriesHelper::CalculateBaseDirectories(char* system_arg_zero)
{
    m_binary_path = std::filesystem::absolute(std::filesystem::path(system_arg_zero));

    std::filesystem::path path_being_validated = m_binary_path;
    while (true)
    {
        if (path_being_validated == path_being_validated.root_path())
        {
            SPDLOG_CRITICAL("Failed to setup base directories, aborting");
            Unicamotor::RequestExit();
            break;
        }

        std::filesystem::path possible_engine_root_path = path_being_validated;
        possible_engine_root_path.append("config/default.ini");
        if (!std::filesystem::exists(possible_engine_root_path))
        {
            path_being_validated = path_being_validated.parent_path();
            continue;
        }

        SPDLOG_INFO("Engine root path found at {}", path_being_validated.string());
        break;
    }

    m_engine_root_directory = path_being_validated;

    /*@TODO When engine + project support exists, create the logic for project directory
     * Keeping it the same as engine root for the moment */
    m_project_root_directory = path_being_validated;
}

std::string DirectoriesHelper::ReadFileAsString(const std::filesystem::path& file_path)
{
    if (!std::filesystem::exists(file_path))
    {
        SPDLOG_ERROR("The file {} does not exist", file_path.string());
        return { };
    }

    std::ifstream file_as_string(file_path.string());
    if (!file_as_string.is_open())
    {
        SPDLOG_ERROR("Can't open the file {}", file_path.string());
        return { };
    }

    std::stringstream string_buffer;
    string_buffer << file_as_string.rdbuf();
    file_as_string.close();

    return string_buffer.str();
}

std::vector<std::filesystem::path> DirectoriesHelper::GetAllFilesInDirectory(const std::filesystem::path& directory, const std::vector<std::string>& extensions)
{
    if (!std::filesystem::exists(directory))
    {
        SPDLOG_ERROR("The directory {} does not exist", directory.string());
        return { };
    }

    std::vector<std::filesystem::path> found_files;
    for (const std::filesystem::directory_entry& path_entry : std::filesystem::recursive_directory_iterator(directory)) 
    {
        if (path_entry.is_directory())
        {
            continue;
        }

        if (extensions.empty())
        {
            found_files.push_back(path_entry);
            continue;
        }
        
        std::string path_extension = path_entry.path().extension().string();
        if (std::find(extensions.begin(), extensions.end(), path_extension) != extensions.end())
        {
            found_files.push_back(path_entry);
            continue;
        }
    }

    return found_files;
}
