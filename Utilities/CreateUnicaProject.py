import sys
import logging
import re
import pathlib
import os
import time

logging.basicConfig(level=logging.INFO, format="%(levelname)s: %(message)s")
unica_root_directory = pathlib.Path(os.path.dirname(__file__)).joinpath("../").resolve()

def project_already_configured_conflict():
    user_answer = input("PROMPT: There's already a project here. Do you want to overwrite it? (y/n): ").lower()
    while user_answer != "y" and user_answer != "n":
        logging.warning("Invalid answer. Use 'y' (yes) or 'n' (no)")
        user_answer = input("PROMPT: There's already a project here. Do you want to overwrite it? (y/n): ").lower()        
    return user_answer == "n"

def set_project_name_in_engine_config(create_project_name: str):
    unica_default_engine_config_file_directory = unica_root_directory.joinpath("Unica/Config/DefaultEngine.ini")

    file_lines: list[str] = []
    with open(unica_default_engine_config_file_directory, "r") as read_default_engine_config_file:
        file_lines = read_default_engine_config_file.readlines()

    line_to_replace_index = -1
    for line in file_lines:
        line_to_replace_index += 1
        if "ProjectName=" in line:
            if (project_already_configured_conflict()):
                logging.info("Will not overwrite existing project. Aborting")
                quit()
            break
    
    if line_to_replace_index < 0:
        line_to_replace_index = len(file_lines)
        file_lines.append("") # Add an empty line to populate the array

    file_lines[line_to_replace_index] = "ProjectName={}\n".format(create_project_name)

    with open(unica_default_engine_config_file_directory, "w+") as write_default_engine_config_file:
        write_default_engine_config_file.writelines(file_lines)

def append_project_directory_to_engine_cmake(create_project_name: str):
    engine_cmake_file_directory = unica_root_directory.joinpath("CMakeLists.txt")
    with open(engine_cmake_file_directory, "a+") as engine_cmake_file:
        engine_cmake_file.write("add_subdirectory(\"{}\")\n".format(create_project_name))

def generate_project_cmake_file(create_project_name: str):
    file_directory = unica_root_directory.joinpath("{}/CMakeLists.txt".format(create_project_name))
    file_directory.parent.mkdir(parents=True, exist_ok=True)

    write_file = open(file_directory, "w+");
    file_contents = f"""cmake_minimum_required(VERSION 3.21)

set(SourceFiles
    Config/DefaultGame.ini
    Source/{create_project_name}.cpp
    Source/{create_project_name}.h
)
add_library("{create_project_name}" ${{SourceFiles}})

# Create file structure in IDE
foreach(SourceFile IN ITEMS ${{SourceFiles}})
    get_filename_component(SourcePath ${{SourceFile}} PATH)
    string(REPLACE ${{CMAKE_SOURCE_DIR}} "" GroupPath ${{SourcePath}})
    string(REPLACE "/" "\\\\" GroupPath ${{GroupPath}})
    source_group(${{GroupPath}} FILES ${{SourceFile}})
endforeach()

find_package(Vulkan REQUIRED FATAL_ERROR)

target_include_directories("{create_project_name}" PUBLIC
    Source/
)

target_compile_definitions({create_project_name} PRIVATE
    $<$<CONFIG:Release>:UNICA_SHIPPING>
    $<$<CONFIG:RelWithDebInfo>:UNICA_SHIPPING>
    $<$<CONFIG:MinSizeRel>:UNICA_SHIPPING>
)"""

    write_file.write(file_contents)
    write_file.close()

def generate_project_empty_files(create_project_name: str):
    files_list = [ "Config/DefaultGame.ini", "Source/{0}.cpp".format(create_project_name), "Source/{0}.h".format(create_project_name) ]
    for file in files_list:
        file_directory = unica_root_directory.joinpath("{0}/{1}".format(create_project_name, file))
        file_directory.parent.mkdir(parents=True, exist_ok=True)
        with open(file_directory, "w+") as write_file:
            write_file.write("\n")

def generate_project_directory_and_files(create_project_name: str):
    generate_project_cmake_file(create_project_name)
    generate_project_empty_files(create_project_name)

if __name__ == "__main__":

    if len(sys.argv) < 2:
        logging.error("A project name wasn't provided as an argument")
        quit()

    create_project_name = sys.argv[1]
    project_name_size = len(create_project_name)
        
    if project_name_size < 4 or project_name_size > 32:
        logging.error("Invalid project name '{}'. It needs to be between 4 and 32 characters long".format(create_project_name))
        quit()

    ensure_valid_project_name_regex = re.compile(r'[a-zA-Z]*')
    if ensure_valid_project_name_regex.fullmatch(create_project_name) == None:
        logging.error("Invalid project name '{}'. Only use ASCII letters".format(create_project_name))
        quit()

    set_project_name_in_engine_config(create_project_name)
    logging.info("Creating Unica project '{}'".format(create_project_name))
    start_creation_time = time.time()

    append_project_directory_to_engine_cmake(create_project_name)

    generate_project_directory_and_files(create_project_name)
    total_creation_time = time.time() - start_creation_time
    logging.info("Finished creating Unica project '{0}' in {1:.2f} seconds".format(create_project_name, total_creation_time))
