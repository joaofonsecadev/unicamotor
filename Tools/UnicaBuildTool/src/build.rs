use crate::GlobalValues;
use tracing::{info, error, trace};
use std::{process::{Command, Stdio}, thread, io::{self, BufRead}};

pub fn build(global_values: &GlobalValues) {
    info!("Building the project using Ninja");

    let init_dir = std::env::current_dir().unwrap();
    let _ch_dir_result = std::env::set_current_dir(global_values.unica_root_path.join("Intermediate").join("Ninja")).unwrap();
    

    let ninja_path = which::which("ninja");
    if ninja_path.is_err() {
        error!("Can't find Ninja in path. Aborting build");
        return;
    }

    let mut command = Command::new("ninja")
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .spawn()
        .unwrap();

    let stdout = command.stdout.take().unwrap();
    let stdout_reader_thread = thread::spawn(move || {
        let reader = io::BufReader::new(stdout);
        for line in reader.lines() {
            trace!("Ninja: {}", line.unwrap());
        }
    });

    let stderr = command.stderr.take().unwrap();
    let stderr_reader_thread = thread::spawn(move || {
        let reader = io::BufReader::new(stderr);
        for line in reader.lines() {
            error!("Ninja: {}", line.unwrap());
        }
    });

    let command_exit_status = command.wait();
    if stdout_reader_thread.join().is_err() {
        error!("Ninja stdout reader thread failed to join");
    }
    if stderr_reader_thread.join().is_err() {
        error!("Ninja stderr reader thread failed to join");
    }

    let exit_status = command_exit_status.unwrap().code().unwrap();
    if exit_status != 0 {
        error!("Ninja failed with exit code {}", exit_status);
    } else {
        info!("Ninja completed successfuly");
    }

    let _ch_dir_original_result = std::env::set_current_dir(init_dir);
}
