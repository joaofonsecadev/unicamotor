// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved

use std::{path::PathBuf};
use tracing::{error, debug, trace};
use walkdir::{WalkDir};

pub fn get_project_root_dir() -> PathBuf{
    let executable_path_result = std::env::current_exe();
    if executable_path_result.is_err() {
        error!("Couldn't get the directory of the executable");
        panic!();
    }
    let executable_path_buffer = executable_path_result.unwrap();
    debug!("Directory of the executable is: {:?}", executable_path_buffer.display());

    let executable_path_ancestors = executable_path_buffer.ancestors();
    for ancestor in executable_path_ancestors {
        trace!("Validating possible Unica project root path '{}'", ancestor.display());

        let unica_folder_exists = ancestor.join("Unica").is_dir();
        if !unica_folder_exists {
            continue;
        }

        let cmake_file_exists = ancestor.join("CMakeLists.txt").is_file();
        if !cmake_file_exists {
            continue;
        }

        debug!("Found a valid Unica project root '{}'", ancestor.display());
        return ancestor.to_path_buf();
    }

    error!("Couldn't find a Unica project root. Is the executable in a subdirectory of it?");
    panic!();
}

pub fn get_files_in_dir(directories: &[&str]) -> Vec<PathBuf> {
    let mut files_to_return: Vec<PathBuf> = vec![];
    for directory in directories {
        for file_entry in WalkDir::new(directory).into_iter().filter_map(|e| e.ok()) {
            files_to_return.push(file_entry.into_path());
        }
    }
    return files_to_return;
}