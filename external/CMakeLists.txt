cmake_minimum_required(VERSION 3.21)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

add_subdirectory(vk-bootstrap)
add_subdirectory(VulkanMemoryAllocator)

add_subdirectory(tomlplusplus)

add_subdirectory(spdlog)
set_target_properties(spdlog PROPERTIES FOLDER external)

option(TRACY_ENABLE "" ON)
option(TRACY_ON_DEMAND "" ON)
option(TRACY_CALLSTACK "" ON)
option(TRACY_ONLY_LOCALHOST "" ON)
add_subdirectory(tracy)
target_compile_options(TracyClient PUBLIC "-w")
set_target_properties(TracyClient PROPERTIES FOLDER external)

add_subdirectory(glfw)

add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)
