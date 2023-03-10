cmake_minimum_required(VERSION 3.21)

project("UnicaSandbox" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Build)

set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(SHADERC_SKIP_TESTS ON CACHE BOOL "" FORCE)
set(SHADERC_SPIRV_TOOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/spirv-tools" CACHE STRING "" FORCE)
set(SHADERC_SPIRV_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/spirv-headers" CACHE STRING "" FORCE)
set(SHADERC_GLSLANG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/glslang" CACHE STRING "" FORCE)

add_subdirectory("Libraries/fmt")
add_subdirectory("Libraries/glm")
add_subdirectory("Libraries/glfw")

add_subdirectory("Libraries/shaderc")
set_property(DIRECTORY "Libraries/shaderc" PROPERTY FOLDER "Libraries/shaderc")

find_package(Vulkan REQUIRED FATAL_ERROR)

add_subdirectory("Unica")
