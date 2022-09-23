cmake_minimum_required(VERSION 3.21)

set(SourceFiles
    Config/DefaultEngine.ini
    Source/Main.cpp
    Source/Core/UnicaMinimal.h
    Source/Log/Logger.cpp
    Source/Log/Logger.h
    Source/Subsystem/SubsystemBase.h
    Source/Subsystem/SubsystemManager.cpp
    Source/Subsystem/SubsystemManager.h
    Source/Timer/TimeManager.cpp
    Source/Timer/TimeManager.h
)

add_executable("Unica" ${SourceFiles})
target_include_directories("Unica" PUBLIC Source/ Source/Core)

# Create file structure in IDE
foreach(SourceFile IN ITEMS ${SourceFiles})
    get_filename_component(SourcePath ${SourceFile} PATH)
    string(REPLACE ${CMAKE_SOURCE_DIR} "" GroupPath ${SourcePath})
    string(REPLACE "/" "\\" GroupPath ${GroupPath})
    source_group(${GroupPath} FILES ${SourceFile})
endforeach()

add_subdirectory("Libraries/fmt")
target_link_libraries("Unica" fmt::fmt)

# @TODO
#
# targetdir ("../Build/" .. outputdir .. "/%{prj.name}")
# objdir ("../Intermediate/" .. outputdir .. "%{prj.name}")
# 
# filter "configurations:Debug"
#     defines "UNICA_DEBUG"
#     symbols "on"
# 
# filter "configurations:Release"
#     defines "UNICA_RELEASE"
#     optimize "on"