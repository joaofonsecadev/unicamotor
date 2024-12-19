cmake_minimum_required(VERSION 3.21)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

add_subdirectory(vk-bootstrap)
add_subdirectory(VulkanMemoryAllocator)

option(TRACY_ENABLE "" ON)
option(TRACY_ON_DEMAND "" ON)
option(TRACY_CALLSTACK "" ON)
option(TRACY_ONLY_LOCALHOST "" ON)
add_subdirectory(tracy)
target_compile_options(TracyClient PUBLIC "-w")

add_subdirectory(glfw)

add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

set_target_properties(TracyClient PROPERTIES FOLDER external)
set_target_properties(glfw PROPERTIES FOLDER external)
set_target_properties(glad PROPERTIES FOLDER external)
set_target_properties(vk-bootstrap PROPERTIES FOLDER external)
