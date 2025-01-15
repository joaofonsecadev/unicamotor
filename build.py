import shutil
import argparse
import subprocess
import hashlib
import pathlib

import os
_ = os.system('color')

def get_command(command: str):
    cmd = shutil.which(command)
    if cmd is None:
        raise Exception(f"{cmd} is not installed or not on PATH")

    return cmd

def is_cmakelists_file_updated(build_type: str) -> bool:
    current_md5 = hashlib.md5()
    with open("CMakeLists.txt", 'rb') as base_cmake_file:
        data = base_cmake_file.read()
        current_md5.update(data)

    pathlib.Path("build").mkdir(exist_ok=True)
    saved_file_path = pathlib.Path("build/buildtool.txt")
    if not saved_file_path.exists():
        with open(saved_file_path, "w") as saved_file:
            _ = saved_file.write(f"{build_type}\n{current_md5.hexdigest()}")
            return True
       
    old_build_type = ""
    old_md5 = ""
    with open(saved_file_path, "r") as saved_file:
        build_tool_file_str = saved_file.read()
        line_by_line_build_tool_file = build_tool_file_str.splitlines()
        if len(line_by_line_build_tool_file) == 2:
            old_build_type = line_by_line_build_tool_file[0]
            old_md5 = line_by_line_build_tool_file[1]


    if current_md5.hexdigest() == old_md5 and build_type == old_build_type:
        return False

    with open(saved_file_path, "w") as saved_file:
        _ = saved_file.write(f"{build_type}\n{current_md5.hexdigest()}")

    return True

def cmake_run(args: argparse.Namespace):
    if not is_cmakelists_file_updated(args.build_type):
        #print("No CMakeLists.txt updates, skipping build files generation\n")
        return False
    cmake_path = get_command("cmake")
    command_fmt = f"{cmake_path} -S . -B build/ -G Ninja -DCMAKE_BUILD_TYPE={args.build_type}"
    print(f"Generating CMake files: {command_fmt}\033[90m")
    try:
        _ = subprocess.run(command_fmt, check=True)
    except subprocess.CalledProcessError:
        os.remove(pathlib.Path("build/buildtool.txt"))
        print("\033[31mCMake failure! Aborting\033[0m")
        exit(-1)
    print("\033[0m")
    return True

def generate_compile_commands(args: argparse.Namespace, force_run: bool):
    if not is_cmakelists_file_updated(args.build_type) and not force_run:
        #print("No CMakeLists.txt updates, skipping compile_commands.json generation\n")
        return
    ninja_path = get_command("ninja")
    command_fmt = f"{ninja_path} -C build/ -t compdb"
    print(f"Generating compile_commands.json: {command_fmt}\033[90m")
    delete_command_file = False
    with open("compile_commands.json", "w") as compile_commands_file:
        try:
            _ = subprocess.run(command_fmt, stdout=compile_commands_file, check=True)
        except subprocess.CalledProcessError:
            delete_command_file = True

    if delete_command_file:
        os.remove("compile_commands.json")
        print("\033[31mFailed generation of compile_commands.json! Aborting\033[0m")
        exit(-1)

    print("\033[0m")

def build_project():
    ninja_path = get_command("ninja")
    command_fmt = f"{ninja_path} -C build/"
    print(f"Building: {command_fmt}\033[90m")
    try:
        _ = subprocess.run(command_fmt, check=True)
    except subprocess.CalledProcessError:
        print("\033[31mBuild failure! Aborting\033[0m")
        exit(-1)
    print("\033[0m")

def run_tests() -> bool:
    binary_path = ""
    if os.name == "nt":
        binary_path = "bin/unicamotor_tests.exe"
    else:
        raise Exception("Missing implementation for this OS")

    command = f"{binary_path}"
    print(f"Starting test run: {command}\033[90m")
    try:
        _ = subprocess.run(command, check=True)
    except subprocess.CalledProcessError:
        print("\033[31mTests failure! Aborting\033[0m")
        exit(-1)
    print("\033[0m")

    return True

def start_project():
    binary_path = ""
    if os.name == "nt":
        binary_path = pathlib.Path("bin/unicamotor.exe")
    else:
        raise Exception("Missing implementation for this OS")

    os.startfile(binary_path)
    return

def main():
    argument_parser = argparse.ArgumentParser(
        prog="Unicamotor Build System",
        description="Build system for the amazing Unicamotor, the best game engine which will never be")
    _ = argument_parser.add_argument("-c", "--cmake-run", required=False, action="store_true", help="Generates build files using ninja by default")
    _ = argument_parser.add_argument('--build-only', help='Builds the project using ninja by default', required=False, action='store_true')
    _ = argument_parser.add_argument("-t", "--build-type", help="Specify the build type. Defaults to Debug", choices=["Debug", "Release", "RelWithDebInfo", "MinSizeRel"], default="Debug")
    _ = argument_parser.add_argument("--skip-tests", help="Skips running the catch2 test suite", required=False, action='store_true')
    _ = argument_parser.add_argument("--no-run", help="Does not execute the project at the end", required=False, action='store_true')
    parsed_arguments: argparse.Namespace = argument_parser.parse_args()

    if parsed_arguments.cmake_run:
        did_cmake_run = cmake_run(parsed_arguments)
        generate_compile_commands(parsed_arguments, did_cmake_run)
        return
    elif parsed_arguments.build_only:
        build_project()
        return
    else:
        did_cmake_run = cmake_run(parsed_arguments)
        generate_compile_commands(parsed_arguments, did_cmake_run)
        build_project()

    if not parsed_arguments.skip_tests:
        run_tests()

    if not parsed_arguments.no_run:
        start_project()

if __name__ == "__main__":
    main()
