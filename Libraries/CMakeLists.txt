cmake_minimum_required(VERSION 3.21)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(SHADERC_SKIP_TESTS ON CACHE BOOL "" FORCE)
set(SHADERC_SKIP_EXAMPLES ON CACHE BOOL "" FORCE)
set(SHADERC_SPIRV_TOOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/spirv-tools" CACHE STRING "" FORCE)
set(SHADERC_SPIRV_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/spirv-headers" CACHE STRING "" FORCE)
set(SHADERC_GLSLANG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glslang" CACHE STRING "" FORCE)

set(SPIRV_SKIP_TESTS ON CACHE BOOL "" FORCE)
set(SPIRV_SKIP_EXECUTABLES ON CACHE BOOL "" FORCE)

add_subdirectory("fmt")
add_subdirectory("glm")
add_subdirectory("sdl")
add_subdirectory("spdlog")

set_target_properties(spdlog PROPERTIES FOLDER "Libraries")
set_target_properties(fmt PROPERTIES FOLDER "Libraries")
set_target_properties(SDL3-shared PROPERTIES FOLDER "Libraries")
