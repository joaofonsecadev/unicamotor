use std::{path::Path, io::{BufReader, BufRead}, fs};
use regex::Regex;
use tracing::{error, warn, info};

use crate::static_files;

fn update_base_engine(project_name: &str, overwrite_existing_project: bool, unica_root_path: &Path) -> bool {
    let engine_base_config_path = unica_root_path.join("Unica/Config/BaseEngine.ini");
    if !engine_base_config_path.exists() {
        warn!("File {} does not exist. Creating a new one", engine_base_config_path.to_str().unwrap());
        if std::fs::File::create(&engine_base_config_path).is_err() {
            error!("Couldn't create new file '{}'", engine_base_config_path.to_str().unwrap());
            return false;
        }
    }

    let read_engine_base_config = std::fs::File::open(&engine_base_config_path);

    if read_engine_base_config.is_err() {
        error!("Couldn't read file '{}'", &engine_base_config_path.to_str().unwrap());
        return false;
    }

    let mut engine_base_config_lines: Vec<String> = BufReader::new(read_engine_base_config.unwrap())
        .lines()
        .map(|l| l.expect("Could not parse line"))
        .collect();

    let mut found_project_name_line = false;
    let mut project_name_line_index: usize = 0;
    for line in engine_base_config_lines.clone() {
        if line.contains("ProjectName=") {
            found_project_name_line = true;
            if !overwrite_existing_project {
                error!("There's an existing Unica project configured. Aborting creation");
                return false;
            }
            break;
        }
        project_name_line_index += 1;
    }

    let project_name_config_string = format!("ProjectName={}", project_name);
    if found_project_name_line {
        engine_base_config_lines[project_name_line_index] = project_name_config_string;
    } else {
        engine_base_config_lines.push(project_name_config_string);
    }

    let write_engine_base_config = std::fs::write(&engine_base_config_path, engine_base_config_lines.join("\n"));
    if write_engine_base_config.is_err() {
        error!("Couldn't write to file '{}'", &engine_base_config_path.to_str().unwrap());
        return false;
    }
    return true;
}

fn update_unica_cmake(project_name: &str, unica_root_path: &Path) -> bool {
    let unica_cmake_file = std::fs::File::open(unica_root_path.join("CMakeLists.txt"));
    let mut unica_cmake_lines: Vec<String> = BufReader::new(unica_cmake_file.unwrap())
        .lines()
        .map(|l| l.expect("Could not parse line"))
        .collect();

    let mut does_cmake_already_have_project = false;
    let project_cmake_string = format!("add_subdirectory({})", project_name);
    for line in unica_cmake_lines.clone() {
        if line.contains(&project_cmake_string) {
            does_cmake_already_have_project = true;
            break;
        }
    }

    if does_cmake_already_have_project {
        return true;
    }

    unica_cmake_lines.push(project_cmake_string);
    let write_unica_cmake = std::fs::write(unica_root_path.join("CMakeLists.txt"), unica_cmake_lines.join("\n"));
    if write_unica_cmake.is_err() {
        error!("Couldn't write to file CMakeLists.txt");
        return false;
    }

    return true;
}

fn generate_project_files(project_name: &str, unica_root_path: &Path) -> bool {
    if fs::create_dir_all(unica_root_path.join(format!("{}/Source", project_name))).is_err() {
        error!("Couldn't create directory {}/Source", project_name);
        return false;
    }

    if fs::create_dir_all(unica_root_path.join(format!("{}/Config", project_name))).is_err() {
        error!("Couldn't create directory {}/Config", project_name);
        return false;
    }

    if std::fs::write(unica_root_path.join(format!("{}/CMakeLists.txt", project_name)), static_files::cmake::get_cmake(project_name)).is_err() {
        error!("Couldn't create file {}/CMakeLists.txt", project_name);
        return false;
    }

    if std::fs::write(unica_root_path.join(format!("{}/Source/{}.h", project_name, project_name)), static_files::header::get_header(project_name)).is_err() {
        error!("Couldn't create file {}/Source/{}.txt", project_name, project_name);
        return false;
    }

    if std::fs::write(unica_root_path.join(format!("{}/Source/{}.cpp", project_name, project_name)), static_files::source::get_source(project_name)).is_err() {
        error!("Couldn't create file {}/Source/{}.txt", project_name, project_name);
        return false;
    }

    if std::fs::write(unica_root_path.join(format!("{}/Config/DefaultGame.ini", project_name)), static_files::config::get_config(project_name)).is_err() {
        error!("Couldn't create file {}/Config/DefaultGame.ini", project_name);
        return false;
    }

    return true;
}

pub fn create_unica_project(project_name: &str, overwrite_existing_project: bool, unica_root_path: &Path) {
    info!("Creating Unica project with name '{}'", project_name);
    
    if project_name.is_empty() {
        error!("Provided project name is empty");
        panic!();
    }

    let project_name_length = project_name.len();
    if project_name_length < 3 || project_name_length > 32 {
        error!("Invalid project name. It needs to be between 3 and 32 characters long");
        panic!();
    }

    let valid_project_name_regex = Regex::new(r"[a-zA-Z]*").unwrap();
    if !valid_project_name_regex.is_match(project_name) {
        error!("Invalid project name. Only use ASCII letters");
        panic!();
    }

    if !update_unica_cmake(project_name, unica_root_path) {
        error!("Failed to update Unica/CMakeLists.txt for project '{}'", &project_name);
        panic!();
    }

    if !update_base_engine(project_name, overwrite_existing_project, unica_root_path) {
        error!("Failed to update BaseEngine.ini config file for project '{}'", &project_name);
        panic!();
    }

    if !generate_project_files(project_name, unica_root_path) {
        error!("Failed to create project files for project '{}'", &project_name);
        panic!();
    }

    info!("Successfuly create project files for '{}'", project_name);
}