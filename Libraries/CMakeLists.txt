cmake_minimum_required(VERSION 3.21)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

add_subdirectory("fmt")
add_subdirectory("glm")
add_subdirectory("sdl")
add_subdirectory("sdl_image")
add_subdirectory("spdlog")

set_target_properties(spdlog PROPERTIES FOLDER "Libraries")
set_target_properties(fmt PROPERTIES FOLDER "Libraries")
set_target_properties(SDL3-shared PROPERTIES FOLDER "Libraries")
set_target_properties(SDL3_image PROPERTIES FOLDER "Libraries")

