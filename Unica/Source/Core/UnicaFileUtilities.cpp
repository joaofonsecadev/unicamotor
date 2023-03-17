#include "UnicaFileUtilities.h"

#include <fstream>
#include <fmt/format.h>

#include "UnicaInstance.h"
#include "Log/Logger.h"

#ifdef __APPLE__
#include <sstream>
#endif

std::filesystem::path UnicaFileUtilities::ResolveDirectory(std::string FileLocation)
{
    std::filesystem::path UnicaFilePath(FileLocation);
    if (UnicaFilePath.is_absolute())
    {
        return UnicaFilePath;
    }
    
    std::filesystem::path BaseDirectory = UnicaInstance::GetProjectRootDirectory();
    const std::string EnginePrefix = "Engine:";
    const std::string GamePrefix = "Game:";
    
    if (!FileLocation.rfind(EnginePrefix, 0))
    {
        BaseDirectory.append("Unica");
        FileLocation.erase(0, EnginePrefix.length());
    }
    else if (!FileLocation.rfind(GamePrefix, 0))
    {
        BaseDirectory.append("Game");
        FileLocation.erase(0, GamePrefix.length());
    }
    else
    {
        const std::string ErrorMessage = fmt::format(
            "Provided file location '{}' lacks a prefix '{}' or '{}', according to its location",
            FileLocation, EnginePrefix, GamePrefix);
        
        UNICA_LOG(Error, __FUNCTION__, ErrorMessage);
    }
    
    UnicaFilePath = std::filesystem::path(FileLocation);
    UnicaFilePath.make_preferred();
    return BaseDirectory.append(UnicaFilePath.c_str());
}

std::vector<std::filesystem::path> UnicaFileUtilities::GetFilesInPathWithExtension(const std::string& PathToSearchString, const std::string& FileExtensionString)
{
    std::vector<std::string> FileExtension(1);
    FileExtension[0] = FileExtensionString;
    return GetFilesInPathWithExtension(PathToSearchString, FileExtension);
}

std::vector<std::filesystem::path> UnicaFileUtilities::GetFilesInPathWithExtension(const std::string& PathToSearchString, const std::vector<std::string>& FileExtensions)
{
    std::vector<std::filesystem::path> FinalFilesVector;
    const std::filesystem::path PathToSearch = ResolveDirectory(PathToSearchString);

    if (!std::filesystem::is_directory(PathToSearch))
    {
        const std::string ErrorMessage = fmt::format("Path '{}' is not valid", PathToSearch.string());
        UNICA_LOG(Error, __FUNCTION__, ErrorMessage);
        return FinalFilesVector;
    }
    
    for (const std::filesystem::directory_entry& PathEntry : std::filesystem::recursive_directory_iterator(PathToSearch))
    {
        if (PathEntry.is_directory())
        {
            continue;
        }

        std::filesystem::path PathEntryExtension = PathEntry.path().extension();
        for (const std::string& FileExtension : FileExtensions)
        {
            if (PathEntryExtension == FileExtension)
            {
                FinalFilesVector.push_back(PathEntry);
                break;
            }
        }
    }
    
    return FinalFilesVector;
}

std::vector<char> UnicaFileUtilities::ReadFileAsBinary(const std::string& FileLocation)
{
    const std::filesystem::path FileDirectory = ResolveDirectory(FileLocation);

#ifdef __APPLE__
    std::ifstream FileAsBinary(FileDirectory.c_str(), std::ios::ate);
#else
    std::ifstream FileAsBinary(FileDirectory.c_str(), std::ios::ate | std::ios::binary);
#endif

    if (!FileAsBinary.is_open())
    {
        UNICA_LOG(Error, __FUNCTION__, fmt::format("Can't open file '{}'", FileDirectory.string()));
        return { };
    }

    const std::streamsize FileSize = FileAsBinary.tellg();
    std::vector<char> Buffer(FileSize);

    FileAsBinary.seekg(0);
    FileAsBinary.read(Buffer.data(), FileSize);
    
    return Buffer;
}

std::string UnicaFileUtilities::ReadFileAsString(const std::string& FileLocation)
{
    const std::filesystem::path FileDirectory = ResolveDirectory(FileLocation);
    
    std::ifstream FileAsString(FileLocation);
    if (!FileAsString.is_open())
    {
        UNICA_LOG(Error, __FUNCTION__, fmt::format("Can't open file '{}'", FileDirectory.string()));
        return "";
    }

#ifdef __APPLE__
    std::basic_stringstream<char> StringBuffer;
#else
    std::stringstream StringBuffer;
#endif

    StringBuffer << FileAsString.rdbuf();
    FileAsString.close();
    return StringBuffer.str();
}

bool UnicaFileUtilities::WriteFile(const std::vector<char>& FileSource, const std::string& FileDestination)
{
    const std::filesystem::path FileDirectory = ResolveDirectory(FileDestination);
    std::ofstream OutputFile(FileDirectory.string(), std::ios::trunc);

    if (!OutputFile)
    {
        UNICA_LOG(Error, __FUNCTION__, fmt::format("Can't open file '{}' for writting", FileDirectory.string()));
        return false;
    }

    OutputFile.write(FileSource.data(), static_cast<std::streamsize>(FileSource.size()));
    return true;
}

bool UnicaFileUtilities::WriteFile(const std::string& FileSource, const std::string& FileDestination)
{
    const std::vector<char> StringToCharVector(FileSource.begin(), FileSource.end());
    return WriteFile(StringToCharVector, FileDestination);
}
