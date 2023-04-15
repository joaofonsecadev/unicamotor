use std::{
    env,
    io::{self, BufRead, BufReader},
    path::{Path, PathBuf},
    process::{Command, Stdio},
    thread,
};
use tracing::{debug, error, info, trace, warn};

use crate::{utils, GlobalValues};

fn get_cmake_path() -> Option<String> {
    let cmake_path = which::which("cmake");
    if cmake_path.is_err() {
        error!("Can't find CMake in $PATH");
        return None;
    }

    debug!(
        "Found CMake at '{}'",
        cmake_path.as_ref().unwrap().display()
    );
    return Some(cmake_path.unwrap().to_str().unwrap().to_string());
}

fn create_compile_commands(global_values: &GlobalValues) {
    debug!("Attempting to create compile_commands.json, using either Ninja or Make");

    let ninja = which::which("ninja");
    let make = which::which("make");
    let generator_type: &str;

    if !ninja.is_err() {
        generator_type = "Ninja";
    }
    else if !make.is_err() {
        generator_type = "Unix Makefiles";
    }
    else {
        warn!("Can't create compile_commands.json, is Ninja or Make available?");
        return;
    }

    spawn_cmake_process(get_cmake_path().unwrap().as_str(), generator_type, "Intermediate/CompileCommands", &global_values.unica_root_path);

    let rename_result = std::fs::rename(global_values.unica_root_path.join("Intermediate/CompileCommands/compile_commands.json"), global_values.unica_root_path.join("compile_commands.json"));
    if rename_result.is_err() {
        warn!("Couldn't move the compile_commands.json file from Intermediate/CompileCommands to the project root");
    }
    info!("Successfuly created compile_commands.json and moved it to the project root");
}

fn spawn_cmake_process(cmake_path: &str, generator_type: &str, dest_path: &str, project_path: &PathBuf) -> bool {
    info!(
        "Executing CMake: {} -S{} -B{} -G{} -DCMAKE_EXPORT_COMPILE_COMMANDS=1",
        cmake_path,
        project_path.to_str().unwrap(),
        project_path.join(dest_path).to_str().unwrap(),
        generator_type
    );

    let mut command = Command::new(cmake_path)
        .arg(format!("-S{}", project_path.to_str().unwrap()))
        .arg(format!(
            "-B{}",
            project_path.join(dest_path).to_str().unwrap()
        ))
        .arg(format!("-G{}", generator_type))
        .arg("-DCMAKE_EXPORT_COMPILE_COMMANDS=1")
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .spawn()
        .unwrap();

    let stdout = command.stdout.take().unwrap();
    let stdout_reader_thread = thread::spawn(move || {
        let reader = io::BufReader::new(stdout);
        for line in reader.lines() {
            trace!("CMake: {}", line.unwrap());
        }
    });

    let stderr = command.stderr.take().unwrap();
    let stderr_reader_thread = thread::spawn(move || {
        let reader = io::BufReader::new(stderr);
        for line in reader.lines() {
            error!("CMake: {}", line.unwrap());
        }
    });

    let command_exit_status = command.wait();
    if stdout_reader_thread.join().is_err() {
        error!("CMake stdout reader thread failed to join");
    }
    if stderr_reader_thread.join().is_err() {
        error!("CMake stderr reader thread failed to join");
    }

    let exit_status = command_exit_status.unwrap().code().unwrap();
    if exit_status != 0 {
        error!("CMake failed with exit code {}", exit_status);
    } else {
        info!("CMake completed successfuly");
    }

    return true;
}

fn generate_solution_files_list(path: &Path) -> Vec<String> {
    let mut files_to_return: Vec<String> = vec![];

    let directory_to_search = [path.to_str().unwrap()];
    let found_files = utils::get_files_in_dir(&directory_to_search);
    for found_file in found_files {
        if !found_file.to_str().unwrap().ends_with(".cpp")
            && !found_file.to_str().unwrap().ends_with(".h")
            && !found_file.to_str().unwrap().ends_with(".cpp")
            && !found_file.to_str().unwrap().ends_with(".frag")
            && !found_file.to_str().unwrap().ends_with(".vert")
            && !found_file.to_str().unwrap().ends_with(".ini")
        {
            continue;
        }
        let mut found_source_file_string = found_file
            .to_str()
            .unwrap()
            .replace(path.to_str().unwrap(), "")
            .replace("\\", "/");
        found_source_file_string.remove(0);
        trace!(
            "Adding file to solution '{}': {}",
            path.components()
                .last()
                .unwrap()
                .as_os_str()
                .to_str()
                .unwrap(),
            &found_source_file_string.as_str()
        );
        files_to_return.push(found_source_file_string);
    }

    files_to_return.sort();
    return files_to_return;
}

fn update_cmake_solution_files(path: &Path, source_files: &Vec<String>) {
    let cmake_file = path.join("CMakeLists.txt");
    let read_file = std::fs::File::open(&cmake_file);

    if read_file.is_err() {
        error!("Couldn't open file '{}'", &cmake_file.to_str().unwrap());
        return;
    }

    let lines: Vec<String> = BufReader::new(read_file.unwrap())
        .lines()
        .map(|l| l.expect("Could not parse line"))
        .collect();

    let mut final_lines: Vec<String> = vec![];
    let mut ignore_line = false;
    for line in lines {
        if ignore_line && line.eq(")") {
            ignore_line = false;
        }

        if ignore_line {
            continue;
        }

        final_lines.push(line.clone());
        if line.eq("set(SourceFiles") {
            for source_line in source_files {
                final_lines.push(format!("    {}", source_line));
            }
            ignore_line = true;
        }
    }

    let file_written = std::fs::write(cmake_file.clone(), final_lines.join("\n"));
    if file_written.is_err() {
        error!("Failed to write updated CMakeLists.txt file '{}'", cmake_file.to_str().unwrap());
    }

    info!("Updated CMakeLists.txt file '{}'", cmake_file.to_str().unwrap());
}

fn get_project_name(unica_root_path: &Path) -> String {
    let base_engine_file = unica_root_path.join("Unica/Config/BaseEngine.ini");
    let read_base_config = std::fs::File::open(&base_engine_file);
    if read_base_config.is_err() {
        error!("Couldn't open file '{}'", &base_engine_file.to_str().unwrap());
        return "".to_string();
    }

    let base_config_lines: Vec<String> = BufReader::new(read_base_config.unwrap())
        .lines()
        .map(|l| l.expect("Could not parse line"))
        .collect();

    for line in base_config_lines {
        if line.contains("ProjectName=") {
            return line.replace("ProjectName=", "").replace("#gitignore", "");
        }
    }

    return "".to_string();
}

fn populate_source_files_list(unica_root_path: &Path) {
    let mut paths_to_process: Vec<PathBuf> = [unica_root_path.join("Unica")].to_vec();
    let project_name = get_project_name(unica_root_path);
    if !project_name.is_empty() {
        paths_to_process.push(unica_root_path.join(project_name))
    }
    for path_to_process in paths_to_process {
        let solution_files = generate_solution_files_list(&path_to_process);
        update_cmake_solution_files(&path_to_process, &solution_files);
    }
}

pub fn get_default_cmake_generator() -> String {
    let operating_system = env::consts::OS;

    match operating_system {
        "windows" => return "Visual Studio 16 2019".to_string(),
        "macos" => return "Xcode".to_string(),
        _ => {
            warn!("Defaulted to an empty solution generator type");
            return String::new();
        }
    }
}

pub fn generate_solution(generator_type: &str, global_values: &GlobalValues) -> () {
    info!("Generating solution files");
    populate_source_files_list(&global_values.unica_root_path);
    let cmake_path = get_cmake_path();
    if cmake_path.is_none() {
        return;
    }

    if generator_type.is_empty() {
        error!("Won't generate solution with an empty generator type. Check supported types in https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html");
        return;
    }

    if !spawn_cmake_process(
        cmake_path.unwrap().as_str(),
        generator_type,
        "Intermediate",
        &global_values.unica_root_path,
    ) {
        error!("CMake error while generating solution files");
        return;
    }

    create_compile_commands(global_values);
}
