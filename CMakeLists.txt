cmake_minimum_required(VERSION 3.21)

project("UnicaSandbox" LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Build)
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_subdirectory("Libraries" EXCLUDE_FROM_ALL)
add_subdirectory("Tools" EXCLUDE_FROM_ALL)

add_subdirectory("Unica")
