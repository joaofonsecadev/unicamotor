cmake_minimum_required(VERSION 3.21)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_subdirectory(spdlog)
set_target_properties(spdlog PROPERTIES FOLDER external)

option(TRACY_ENABLE "" ON)
option(TRACY_ON_DEMAND "" ON)
option(TRACY_CALLSTACK "" ON)
option(TRACY_ONLY_LOCALHOST "" ON)
add_subdirectory(tracy)
target_compile_options(TracyClient PRIVATE "-w")
set_target_properties(TracyClient PROPERTIES FOLDER external)

add_subdirectory(glfw)

add_library(glad glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)
