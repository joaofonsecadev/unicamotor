pub fn get_cmake(project_name: &str) -> String {
    return r#"cmake_minimum_required(VERSION 3.21)

project("CUSTOM_PROJECT_NAME_GOES_HERE" LANGUAGES CXX)

set(SourceFiles
    Config/DefaultGame.ini
    Source/CUSTOM_PROJECT_NAME_GOES_HERE.h
)
add_library("CUSTOM_PROJECT_NAME_GOES_HERE" ${SourceFiles})
    
# Create file structure in IDE
foreach(SourceFile IN ITEMS ${SourceFiles})
    get_filename_component(SourcePath ${SourceFile} PATH)
    string(REPLACE ${CMAKE_SOURCE_DIR} "" GroupPath ${SourcePath})
    string(REPLACE "/" "\\" GroupPath ${GroupPath})
    source_group(${GroupPath} FILES ${SourceFile})
endforeach()

target_include_directories("CUSTOM_PROJECT_NAME_GOES_HERE" PUBLIC
    Source/
)"#.replace("CUSTOM_PROJECT_NAME_GOES_HERE", &project_name).to_string();
}