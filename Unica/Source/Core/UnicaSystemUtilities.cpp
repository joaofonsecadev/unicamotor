#include "UnicaSystemUtilities.h"

#include <fstream>

#include "UnicaInstance.h"

std::filesystem::path UnicaSystemUtilities::ResolveUnicaFileLocation(const std::string& UnicaFileLocation)
{
    std::filesystem::path BaseDirectory = UnicaInstance::GetUnicaRootDirectory();
    
    std::filesystem::path UnicaFilePath(UnicaFileLocation);
    UnicaFilePath.make_preferred();
    
    return BaseDirectory.append(UnicaFilePath.c_str());
}

std::vector<char> UnicaSystemUtilities::ReadFileAsBytes(const std::string& UnicaFileLocation)
{
    const std::filesystem::path FileDirectory = ResolveUnicaFileLocation(UnicaFileLocation);
    std::ifstream FileAsBytes(FileDirectory.c_str(), std::ios::ate, std::ios::binary);

    const std::streamsize FileSize = FileAsBytes.tellg();
    std::vector<char> Buffer(FileSize);

    FileAsBytes.seekg(0);
    FileAsBytes.read(Buffer.data(), FileSize);
    
    return Buffer;
}
