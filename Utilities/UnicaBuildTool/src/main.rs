use std::path::PathBuf;
use clap::Parser;
use tracing::{info, metadata::LevelFilter, debug};
use tracing_subscriber::FmtSubscriber;
use crate::utils::get_project_root_dir;

mod copyright_disclaimer;
mod utils;

#[derive(Parser)]
#[command(author, version, about, long_about = None)]
struct CliArgs {
    /// Create a new Unica project with a given name
    #[arg(short, long, value_name = "PROJECT_NAME")]
    create_unica_project: Option<String>,

    /// Generate OS appropriate solution files
    #[arg(short, long)]
    generate_solution_files: bool,

    /// Write/update the copyright disclaimer in source files
    #[arg(long, value_name = "PROJECT_NAME")]
    write_copyright_disclaimer: bool
}

pub struct GlobalValues {
    unica_root_path: PathBuf
}

fn configure_tracing() {
    tracing::subscriber::set_global_default(FmtSubscriber::builder()
        .with_max_level(LevelFilter::TRACE)
        .with_target(false)
        .with_timer(tracing_subscriber::fmt::time::uptime())
        .finish()).expect("Failed tracing subscriber creation");
}

fn main() {
    configure_tracing();
    info!("Version {}", env!("CARGO_PKG_VERSION"));

    let global_values = GlobalValues { unica_root_path: get_project_root_dir() };
    
    let command_line_args = CliArgs::parse();

    let mut will_generate_solution_files = command_line_args.generate_solution_files;

    if command_line_args.create_unica_project.is_some() {
        debug!("Forcing generation of solution files");
        will_generate_solution_files = true;
    }
    if will_generate_solution_files {
    }
    if command_line_args.write_copyright_disclaimer {
        copyright_disclaimer::write_copyright_disclaimer(global_values);
    }
}
