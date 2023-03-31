use crate::{utils, GlobalValues};
use shaderc;
use std::{io::Write, path::PathBuf};
use tracing::{error, info, trace, debug};

pub fn compile_shaders(global_values: &GlobalValues) {
    info!("Starting shader compilation to SPIRV");
    let directories_to_find_files_list = [global_values.unica_root_path.to_str().unwrap()];

    let mut shader_files: Vec<PathBuf> = vec![];
    let found_files = utils::get_files_in_dir(&directories_to_find_files_list);
    for file in found_files {
        if file.to_str().unwrap().ends_with(".frag") || file.to_str().unwrap().ends_with(".vert") {
            trace!("Found shader file '{}'", file.to_str().unwrap());
            shader_files.push(file);
        }
    }

    let compiler = shaderc::Compiler::new().unwrap();
    let compiler_options = shaderc::CompileOptions::new().unwrap();
    for shader_file in shader_files {
        let start_compilation_time = std::time::Instant::now();

        let loaded_uncompiled_shader =
            std::fs::read_to_string(shader_file.as_os_str().to_str().unwrap());
        if loaded_uncompiled_shader.is_err() {
            error!("Can't load shader file {}", shader_file.to_str().unwrap());
            continue;
        }

        let mut shader_kind = shaderc::ShaderKind::InferFromSource;
        if shader_file.to_str().unwrap().ends_with(".frag") {
            shader_kind = shaderc::ShaderKind::Fragment;
        } else if shader_file.to_str().unwrap().ends_with(".vert") {
            shader_kind = shaderc::ShaderKind::Vertex;
        }

        trace!("Compiling shader file '{}'", shader_file.to_str().unwrap());
        let compiled_shader_binary = compiler
            .compile_into_spirv(
                loaded_uncompiled_shader.ok().unwrap().as_str(),
                shader_kind,
                shader_file.file_name().unwrap().to_str().unwrap(),
                "main",
                Some(&compiler_options),
            )
            .unwrap();

        let output_file_name = format!("{}{}", shader_file.to_str().unwrap(), ".spv");
        let file = std::fs::File::create(output_file_name.clone());
        if file.is_err() {
            error!("Can't write to file {}", output_file_name);
            continue;
        }
        let written_file = file
            .unwrap()
            .write_all(compiled_shader_binary.as_binary_u8());
        if written_file.is_err() {
            error!("Can't write to file {}", output_file_name);
            continue;
        }
        debug!("Compiled shader in {:.0?}: {}", start_compilation_time.elapsed(), output_file_name);
    }
}
