cmake_minimum_required(VERSION 3.21)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_subdirectory(spdlog)
set_target_properties(spdlog PROPERTIES FOLDER external)

option(TRACY_ENABLE "" ON)
option(TRACY_ON_DEMAND "" ON)
add_subdirectory(tracy)
target_compile_options(TracyClient PRIVATE "-w")
set_target_properties(TracyClient PROPERTIES FOLDER external)
