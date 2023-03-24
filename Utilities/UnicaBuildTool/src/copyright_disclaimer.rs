// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved

use std::{fs::File, io::{Error, BufReader, Read, Write}};

use chrono::Datelike;
use tracing::{trace, info, debug, error};
use walkdir::WalkDir;
use crate::GlobalValues;

fn is_file_entry_valid(file_entry: &walkdir::DirEntry) -> bool {
    if !file_entry.file_type().is_file() {
        return false;
    }

    let valid_file_patterns = [
        ".cpp",
        ".h",
        ".frag",
        ".vert",
        ".rs",
        ".py"
    ];

    for valid_pattern in valid_file_patterns {
        if file_entry.file_name().to_string_lossy().ends_with(valid_pattern) {
            return true;
        }
    }

    return false;
}

fn get_comment_character_from_file(file_entry: &walkdir::DirEntry) -> &'static str {
    if file_entry.file_name().to_string_lossy().ends_with(".py") {
        return "#";
    }
    return "//";
}

fn get_file_contents_as_string(file_entry: &walkdir::DirEntry) -> Result<String, Error> {
    let file = File::open(file_entry.path().as_os_str())?;
    let mut buf_reader = BufReader::new(file);
    let mut contents = String::new();
    buf_reader.read_to_string(&mut contents)?;
    return Ok(contents.replace("\r\n", "\n"));
}

fn generate_new_file_contents(input_contents: &str, copyright_disclaimer: &str) -> String {
    if input_contents.starts_with(copyright_disclaimer) {
        return String::new();
    }

    let first_line = input_contents.split("\n").next().unwrap();
    if first_line.to_lowercase().contains("copyright"){
        let ending_contents = input_contents.get(first_line.len()-1..input_contents.len()).unwrap();
        return format!("{}{}", copyright_disclaimer, ending_contents);
    }

    return format!("{}{}{}", copyright_disclaimer, "\n\n", input_contents);
}

fn write_contents_to_file(contents: &str, file_entry: &walkdir::DirEntry) {
    let file = File::create(file_entry.path().as_os_str());
    if file.is_err() {
        error!("Can't write to file {}", file_entry.path().as_os_str().to_str().unwrap());
    }
    let mut ok_file = file.unwrap();
    let write_result = ok_file.write_all(contents.as_bytes());
    if write_result.is_err() {
        error!("Can't write to file {}", file_entry.path().as_os_str().to_str().unwrap());
    }
    trace!("Updated copyright for file {}", file_entry.file_name().to_str().unwrap());
}

pub fn write_copyright_disclaimer(global_values: &GlobalValues) {
    info!("Applying the copyright disclaimer to source files");

    let year = chrono::Utc::now().year();
    let disclaimer = format!("2022-{} Copyright joaofonseca.dev, All Rights Reserved", year);
    debug!("The disclaimer to be written is: '{}'", disclaimer);

    let paths_to_get_files_from = [
        global_values.unica_root_path.join("Unica/Source"),
        global_values.unica_root_path.join("Unica/Shaders"),
        global_values.unica_root_path.join("Utilities")
    ];

    for path_entry in paths_to_get_files_from {
        for file_entry in WalkDir::new(path_entry).into_iter().filter_map(|e| e.ok()) {
            if !is_file_entry_valid(&file_entry) {
                continue;
            }

            let comment_character = get_comment_character_from_file(&file_entry);
            let full_copyright_comment = format!("{} {}", comment_character, disclaimer);

            let file_content_string = get_file_contents_as_string(&file_entry);
            if file_content_string.is_err() {
                continue;
            }

            let ok_file_content_string = file_content_string.unwrap();
            
            let output_content_string = generate_new_file_contents(&ok_file_content_string, &full_copyright_comment);
            if output_content_string.len() == 0 {
                continue;
            }

            write_contents_to_file(&output_content_string, &file_entry);
        }
    }
}