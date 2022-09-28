cmake_minimum_required(VERSION 3.21)

set(SourceFiles
        Config/DefaultEngine.ini
        Source/Main.cpp
        Source/Core/UnicaInstance.cpp
        Source/Core/UnicaInstance.h
        Source/Core/UnicaMinimal.h
        Source/Log/Logger.cpp
        Source/Log/Logger.h
        Source/Renderer/GameWindow.cpp
        Source/Renderer/GameWindow.h
        Source/Renderer/RenderManager.cpp
        Source/Renderer/RenderManager.h
        Source/Subsystem/SubsystemBase.h
        Source/Subsystem/SubsystemManager.cpp
        Source/Subsystem/SubsystemManager.h
        Source/Timer/TimeManager.cpp
        Source/Timer/TimeManager.h
)
add_executable("Unica" ${SourceFiles})

# Create file structure in IDE
foreach(SourceFile IN ITEMS ${SourceFiles})
    get_filename_component(SourcePath ${SourceFile} PATH)
    string(REPLACE ${CMAKE_SOURCE_DIR} "" GroupPath ${SourcePath})
    string(REPLACE "/" "\\" GroupPath ${GroupPath})
    source_group(${GroupPath} FILES ${SourceFile})
endforeach()

# Libraries
add_subdirectory("Libraries/fmt")
add_subdirectory("Libraries/glm")
add_subdirectory("Libraries/glfw")
find_package(Vulkan REQUIRED FATAL_ERROR)

target_include_directories("Unica" PUBLIC
        Source/
        Source/Core
        ${Vulkan_INCLUDE_DIRS}
)
target_link_libraries("Unica"
        glfw
        fmt::fmt
        glm::glm
        ${Vulkan_LIBRARIES}
)

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