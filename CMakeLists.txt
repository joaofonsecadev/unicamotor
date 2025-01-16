cmake_minimum_required(VERSION 3.21)

project(unicamotor LANGUAGES CXX C)
if (PROJECT_IS_TOP_LEVEL)
    set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)
    option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

    set(CMAKE_CXX_STANDARD          17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS        OFF)


    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif()

find_package(Vulkan REQUIRED)
add_subdirectory(external EXCLUDE_FROM_ALL)

# Project source files
set(SourceFiles
        ${CMAKE_SOURCE_DIR}/src/core/queue.cpp
        ${CMAKE_SOURCE_DIR}/src/core/string.cpp
        ${CMAKE_SOURCE_DIR}/src/core/system.cpp
        ${CMAKE_SOURCE_DIR}/src/core/thread.cpp
        ${CMAKE_SOURCE_DIR}/src/logging/logging.cpp
        ${CMAKE_SOURCE_DIR}/src/motor/unicamotor.cpp
        ${CMAKE_SOURCE_DIR}/src/timer/timer.cpp
)

set(HeaderFiles
        ${CMAKE_SOURCE_DIR}/src/core/memory.h
        ${CMAKE_SOURCE_DIR}/src/core/profiling.h
        ${CMAKE_SOURCE_DIR}/src/core/queue.h
        ${CMAKE_SOURCE_DIR}/src/core/string.h
        ${CMAKE_SOURCE_DIR}/src/core/system.h
        ${CMAKE_SOURCE_DIR}/src/core/thread.h
        ${CMAKE_SOURCE_DIR}/src/core/vector.h
        ${CMAKE_SOURCE_DIR}/src/logging/logging.h
        ${CMAKE_SOURCE_DIR}/src/motor/unicaconfig.h
        ${CMAKE_SOURCE_DIR}/src/motor/unicamotor.h
        ${CMAKE_SOURCE_DIR}/src/timer/timer.h
)

set(IncludeDirs
        ${CMAKE_SOURCE_DIR}/src
        ${Vulkan_INCLUDE_DIRS}
)

set(LibsToLink
        glad
        glfw
        Tracy::TracyClient
        vk-bootstrap
        VulkanMemoryAllocator
        ${Vulkan_LIBRARIES}
)

set(AllFiles
        ${HeaderFiles}
        ${SourceFiles}
        ${CMAKE_SOURCE_DIR}/src/main.cpp
)

add_library(unicamotor_lib ${SourceFiles})
target_link_libraries(unicamotor_lib PUBLIC ${LibsToLink})
target_include_directories(unicamotor_lib PUBLIC ${IncludeDirs})
set_target_properties(unicamotor_lib PROPERTIES FOLDER tools)

add_executable(unicamotor ${AllFiles})
target_link_libraries(unicamotor PRIVATE unicamotor_lib)

if (PROJECT_IS_TOP_LEVEL)
    add_subdirectory(external/catch2)
    set_target_properties(Catch2WithMain PROPERTIES FOLDER external)
    set_target_properties(Catch2 PROPERTIES FOLDER external)

    set(TestFiles
            ${CMAKE_SOURCE_DIR}/tests/unique_ptr.cpp
            ${CMAKE_SOURCE_DIR}/tests/vector.cpp
    )

    add_executable(unicamotor_tests ${TestFiles})
    target_link_libraries(unicamotor_tests PRIVATE Catch2::Catch2WithMain unicamotor_lib)
    set_target_properties(unicamotor_tests PROPERTIES FOLDER tools)

    set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)

    include(CTest)
    include(Catch)
    catch_discover_tests(unicamotor_tests)

    foreach(SourceFile IN ITEMS ${TestFiles})
        get_filename_component(SourcePath ${SourceFile} PATH)
        string(REPLACE ${CMAKE_SOURCE_DIR} "" GroupPath ${SourcePath})
        string(REPLACE "/" "\\" GroupPath ${GroupPath})
        source_group(${GroupPath} FILES ${SourceFile})
    endforeach()
endif()

foreach(SourceFile IN ITEMS ${AllFiles})
    get_filename_component(SourcePath ${SourceFile} PATH)
    string(REPLACE ${CMAKE_SOURCE_DIR} "" GroupPath ${SourcePath})
    string(REPLACE "/" "\\" GroupPath ${GroupPath})
    source_group(${GroupPath} FILES ${SourceFile})
endforeach()
