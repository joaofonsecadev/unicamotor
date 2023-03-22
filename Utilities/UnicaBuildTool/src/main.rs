use clap::Parser;

#[derive(Parser, Debug)]
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

fn main() {
    let command_line_args = CliArgs::parse();

    let mut will_generate_solution_files = command_line_args.generate_solution_files;

    if command_line_args.create_unica_project.is_some() {
        println!("Create a unica project");
        will_generate_solution_files = true;
    }
    if will_generate_solution_files {
        println!("Generating solution");
    }
    if command_line_args.write_copyright_disclaimer {
        println!("Writing copyright disclaimer");
    }
}
