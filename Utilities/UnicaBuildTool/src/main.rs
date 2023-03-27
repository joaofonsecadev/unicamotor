// 2022-2023 Copyright joaofonseca.dev, All Rights Reserved

use std::path::PathBuf;
use clap::{Parser};
use tracing::{info, metadata::LevelFilter};
use tracing_subscriber::FmtSubscriber;

use crate::create_project::create_unica_project;

mod copyright_disclaimer;
mod generate_solution;
mod compile_shaders;
mod create_project;
mod static_files;
mod utils;

#[derive(Parser)]
#[command(author, version, about, long_about = None)]
struct CliArgs {
    /// Create a new Unica project with a given name
    #[arg(short, long, value_name = "PROJECT_NAME")]
    create_unica_project: Option<String>,

    /// Create a new Unica project with a given name
    #[arg(short = 'C', long, value_name = "PROJECT_NAME")]
    create_unica_project_overwrite: Option<String>,

    /// Generate OS appropriate solution files
    #[arg(short, long)]
    generate_solution: bool,

    /// Generate solution files using the specified generator
    #[arg(short = 'G', long)]
    generate_solution_with_gen_type: Option<String>,

    /// Compile GLSL shaders to SPIRV format
    #[arg(short = 's', long)]
    compile_shaders: bool,

    /// Write/update the copyright disclaimer in source files
    #[arg(short, long)]
    write_copyright_disclaimer: bool
}

pub struct GlobalValues {
    unica_root_path: PathBuf
}

fn configure_tracing() {
    tracing::subscriber::set_global_default(FmtSubscriber::builder()
        .with_max_level(LevelFilter::TRACE)
        .with_target(false)
        .finish()).expect("Failed tracing subscriber creation");
}

fn main() {
    let start_time = std::time::Instant::now();
    configure_tracing();
    info!("Version {}", env!("CARGO_PKG_VERSION"));

    let global_values = GlobalValues { unica_root_path: utils::get_project_root_dir() };
    
    let command_line_args = CliArgs::parse();

    let mut was_project_created = false;
    if command_line_args.create_unica_project.is_some() {
        create_unica_project(&command_line_args.create_unica_project.unwrap(), false, &global_values.unica_root_path);
        was_project_created = true;
    } else if command_line_args.create_unica_project_overwrite.is_some() {
        create_unica_project(&command_line_args.create_unica_project_overwrite.unwrap(), true, &global_values.unica_root_path);
        was_project_created = true;
    }
    
    if command_line_args.generate_solution_with_gen_type.is_some() {
        generate_solution::generate_solution(&command_line_args.generate_solution_with_gen_type.unwrap(), &global_values);
    } else if command_line_args.generate_solution || was_project_created {
        generate_solution::generate_solution(&generate_solution::get_default_cmake_generator(), &global_values);
    }

    if command_line_args.write_copyright_disclaimer {
        copyright_disclaimer::write_copyright_disclaimer(&global_values);
    }

    if command_line_args.compile_shaders {
        compile_shaders::compile_shaders(&global_values);
    }

    info!("Finished execution in {:.0?}", start_time.elapsed());
}
