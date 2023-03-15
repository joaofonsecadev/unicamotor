cmake_minimum_required(VERSION 3.21)

set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(SHADERC_SKIP_TESTS ON CACHE BOOL "" FORCE)
set(SHADERC_SKIP_EXAMPLES ON CACHE BOOL "" FORCE)
set(SHADERC_SPIRV_TOOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/spirv-tools" CACHE STRING "" FORCE)
set(SHADERC_SPIRV_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/spirv-headers" CACHE STRING "" FORCE)
set(SHADERC_GLSLANG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glslang" CACHE STRING "" FORCE)

set(SPIRV_SKIP_TESTS ON CACHE BOOL "" FORCE)
set(SPIRV_SKIP_EXECUTABLES ON CACHE BOOL "" FORCE)

add_subdirectory("fmt")
add_subdirectory("glm")
add_subdirectory("glfw")
add_subdirectory("shaderc")

set_target_properties(fmt PROPERTIES FOLDER "Libraries")
set_target_properties(glfw update_mappings PROPERTIES FOLDER "Libraries/glfw")
set_target_properties(add-copyright build-version check-copyright shaderc_combined-pkg-config
    shaderc_static-pkg-config shaderc-pkg-config glslc glslc_exe shaderc testdata
    shaderc_combined_genfile shaderc_shared shaderc_util PROPERTIES FOLDER "Libraries/shaderc")
set_target_properties(Continuous Experimental Nightly NightlyMemoryCheck GenericCodeGen glslang
    glslang-default-resource-limits MachineIndependent OGLCompiler OSDependent SPIRV SPVRemapper HLSL glslangValidator
    spirv-remap PROPERTIES FOLDER "Libraries/glslang")
set_target_properties(install-headers PROPERTIES FOLDER "Libraries/spirv-headers")
set_target_properties(core_tables enum_string_mapping extinst_tables spirv-tools-pkg-config spirv-tools-shared-pkg-config
    PROPERTIES FOLDER "Libraries/spirv-tools")
set_target_properties(spirv-tools-build-version spirv-tools-header-DebugInfo 
    spirv-tools-header-NonSemanticShaderDebugInfo100 spirv-tools-header-OpenCLDebugInfo100 spv-tools-cldi100
    spv-tools-clspvreflection spv-tools-debuginfo spv-tools-shdi100 spv-tools-spv-amd-gs spv-tools-spv-amd-sb
    spv-tools-spv-amd-sevp spv-tools-spv-amd-stm PROPERTIES FOLDER "Libraries/spirv-tools/build")
set_target_properties(SPIRV-Tools-diff SPIRV-Tools-link SPIRV-Tools-lint SPIRV-Tools-opt SPIRV-Tools-reduce
    SPIRV-Tools-shared SPIRV-Tools-static PROPERTIES FOLDER "Libraries/spirv-tools/libraries")
set_target_properties(spirv-tools-vimsyntax PROPERTIES FOLDER "Libraries/spirv-tools/utilities")
