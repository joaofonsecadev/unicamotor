cmake_minimum_required(VERSION 3.21)

project(unicamotor LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Project source files
set(SourceFiles
        src/main.cpp
        src/core/unicamotor.cpp
        src/core/unicamotor.h
        src/core/version.h
        src/subsystem/subsystem.h
        src/core/arguments.cpp
        src/core/arguments.h
        src/timer/timer.cpp
        src/timer/timer.h
        src/renderer/renderer.h
        src/renderer/opengl/opengl.cpp
        src/renderer/opengl/opengl.h
)

add_subdirectory(external EXCLUDE_FROM_ALL)

add_executable(unicamotor ${SourceFiles})
target_include_directories(unicamotor PRIVATE src)
target_link_libraries(unicamotor PRIVATE spdlog::spdlog Tracy::TracyClient glfw)

# Create file structure in IDE
foreach(SourceFile IN ITEMS ${SourceFiles})
    get_filename_component(SourcePath ${SourceFile} PATH)
    string(REPLACE ${CMAKE_SOURCE_DIR} "" GroupPath ${SourcePath})
    string(REPLACE "/" "\\" GroupPath ${GroupPath})
    source_group(${GroupPath} FILES ${SourceFile})
endforeach()

