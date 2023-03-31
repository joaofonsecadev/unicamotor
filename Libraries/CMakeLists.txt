cmake_minimum_required(VERSION 3.21)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

option(TRACY_ENABLE "" ON)
option(TRACY_ON_DEMAND "" ON)

add_subdirectory("fmt")
add_subdirectory("glm")
add_subdirectory("sdl")
add_subdirectory("spdlog")
add_subdirectory("tracy")

set_target_properties(spdlog PROPERTIES FOLDER "Libraries")
set_target_properties(fmt PROPERTIES FOLDER "Libraries")
set_target_properties(SDL3-shared PROPERTIES FOLDER "Libraries")
set_target_properties(TracyClient PROPERTIES FOLDER "Libraries")
