use std::{env, process::{Command, Stdio}, path::PathBuf, thread, io::{self, BufRead}};
use tracing::{error, warn, debug, info, trace};

use crate::GlobalValues;

fn get_cmake_path() -> Option<String> {
    let cmake_path = which::which("cmake");
    if cmake_path.is_err() {
        error!("Can't find CMake in $PATH");
        return None;
    }
    
    debug!("Found CMake at '{}'", cmake_path.as_ref().unwrap().display());
    return Some(cmake_path.unwrap().to_str().unwrap().to_string());
}

fn spawn_cmake_process(cmake_path: &str, generator_type: &str, project_path: &PathBuf) -> bool {
    debug!("Executing CMake: {} -S{} -B{} -G{}", cmake_path, project_path.to_str().unwrap(), project_path.join("Intermediate").to_str().unwrap(), generator_type);

    let mut command = Command::new(cmake_path)
        .arg(format!("-S{}", project_path.to_str().unwrap()))
        .arg(format!("-B{}", project_path.join("Intermediate").to_str().unwrap()))
        .arg(format!("-G{}", generator_type))
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

    info!("CMake exited with status {:?}", command_exit_status);
    return true;
}

pub fn get_default_cmake_generator() -> String {
    let operating_system = env::consts::OS;

    match operating_system {
        "windows" => return "Visual Studio 16 2019".to_string(),
        "macos" => return "Xcode".to_string(),
        _ => {
            warn!("Defaulted to an empty solution generator type");
            return String::new()
        },
    }
}

pub fn generate_solution(generator_type: &str, global_values: &GlobalValues) -> () {
    info!("Generating solution files");
    let cmake_path = get_cmake_path();
    if cmake_path.is_none() {
        return;
    }

    if generator_type.is_empty() {
        error!("Won't generate solution with an empty generator type. Check supported types in https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html");
        return;
    }

    if !spawn_cmake_process(cmake_path.unwrap().as_str(), generator_type, &global_values.unica_root_path) {
        error!("CMake error while generating solution files");
        return;
    }
}