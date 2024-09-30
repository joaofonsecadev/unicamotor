#include "directories.h"
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
