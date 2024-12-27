import shutil
import argparse
import subprocess
import hashlib

def get_command(command: str):
    cmd = shutil.which(command)
    if cmd is None:
        raise Exception(f"{cmd} is not installed or not on PATH")

    return cmd

def is_cmakelists_file_updated() -> bool:
    current_md5 = hashlib.md5()
    with open("CMakeLists.txt", 'rb') as base_cmake_file:
        data = base_cmake_file.read()
        current_md5.update(data)

    old_md5 = ""
    with open("build/buildtool.txt", "r") as saved_file:
        old_md5 = saved_file.read()

    if current_md5.hexdigest() == old_md5:
        return False

    with open("build/buildtool.txt", "w") as saved_file:
        saved_file.write(current_md5.hexdigest())

    return True

def cmake_run():
    if not is_cmakelists_file_updated():
        print("No CMakeLists.txt updates, skipping build files generation")
        return
    cmake_path = get_command("cmake")
    command_fmt = f"{cmake_path} -S . -B build/ -G Ninja"
    print(f"Generating CMake files: {command_fmt}")
    cmake_process = subprocess.run(command_fmt)

def generate_compile_commands():
    if not is_cmakelists_file_updated():
        print("No CMakeLists.txt updates, skipping compile_commands.json generation")
        return
    ninja_path = get_command("ninja")
    command_fmt = f"{ninja_path} -C build/ -t compdb"
    print(f"Generating compile_commands.json: {command_fmt}")
    with open("compile_commands.json", "w") as compile_commands_file:
        ninja_process = subprocess.run(command_fmt, stdout=compile_commands_file)

def build_project():
    ninja_path = get_command("ninja")
    command_fmt = f"{ninja_path} -C build/"
    print(f"Building: {command_fmt}")
    ninja_process = subprocess.run(command_fmt)

def main():
    argument_parser = argparse.ArgumentParser(
        prog="Selected Assets Cooker",
        description="Cooks and packs assets within the Unreal Editor.")
    argument_parser.add_argument("-c", "--cmake-run", required=False, action="store_true", help="Generates build files using ninja by default")
    argument_parser.add_argument('-b', '--build', help='Builds the project using ninja by default', required=False, action='store_true')
    parsed_arguments = argument_parser.parse_args()

    if parsed_arguments.cmake_run:
        cmake_run()
        generate_compile_commands()
    elif parsed_arguments.build:
        build_project()
    else:
        cmake_run()
        generate_compile_commands()
        build_project()

    return

if __name__ == "__main__":
    main()
