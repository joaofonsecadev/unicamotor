cmake_minimum_required(VERSION 3.21)

project(unicamotor LANGUAGES CXX C)
if (PROJECT_IS_TOP_LEVEL)
    set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)
    option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

    set(CMAKE_CXX_STANDARD          20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS        OFF)


    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif()

# Project source files
set(SourceFiles
        src/main.cpp
	src/motor/unicaconfig.h
	src/motor/unicamotor.h
)

find_package(Vulkan REQUIRED)
add_subdirectory(external EXCLUDE_FROM_ALL)

add_executable(unicamotor ${SourceFiles})
target_include_directories(unicamotor PUBLIC src ${Vulkan_INCLUDE_DIRS})
target_link_libraries(unicamotor PUBLIC Tracy::TracyClient glfw glad ${Vulkan_LIBRARIES} VulkanMemoryAllocator vk-bootstrap)

# Create file structure in IDE
foreach(SourceFile IN ITEMS ${SourceFiles})
    get_filename_component(SourcePath ${SourceFile} PATH)
    string(REPLACE ${CMAKE_SOURCE_DIR} "" GroupPath ${SourcePath})
    string(REPLACE "/" "\\" GroupPath ${GroupPath})
    source_group(${GroupPath} FILES ${SourceFile})
endforeach()
