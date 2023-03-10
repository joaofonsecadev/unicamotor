cmake_minimum_required(VERSION 3.21)

set(SourceFiles
    Config/DefaultEngine.ini
    Source/Core/UnicaFileUtilities.cpp
    Source/Core/UnicaFileUtilities.h
    Source/Core/UnicaInstance.cpp
    Source/Core/UnicaInstance.h
    Source/Core/UnicaMinimal.h
    Source/Core/UnicaSettings.h
    Source/Log/Logger.cpp
    Source/Log/Logger.h
    Source/Main.cpp
    Source/Renderer/RenderManager.cpp
    Source/Renderer/RenderManager.h
    Source/Renderer/RenderWindow.cpp
    Source/Renderer/RenderWindow.h
    Source/Renderer/Vulkan/VulkanAPI.cpp
    Source/Renderer/Vulkan/VulkanAPI.h
    Source/Renderer/Vulkan/VulkanQueueFamilyIndices.cpp
    Source/Renderer/Vulkan/VulkanQueueFamilyIndices.h
    Source/Renderer/Vulkan/VulkanSwapChainSupportDetails.h
    Source/Subsystem/SubsystemBase.h
    Source/Subsystem/SubsystemManager.cpp
    Source/Subsystem/SubsystemManager.h
    Source/Timer/TimeManager.cpp
    Source/Timer/TimeManager.h
)
add_executable("Unica" ${SourceFiles})

# Create file structure in IDE
foreach(SourceFile IN ITEMS ${SourceFiles})
    get_filename_component(SourcePath ${SourceFile} PATH)
    string(REPLACE ${CMAKE_SOURCE_DIR} "" GroupPath ${SourcePath})
    string(REPLACE "/" "\\" GroupPath ${GroupPath})
    source_group(${GroupPath} FILES ${SourceFile})
endforeach()

target_include_directories("Unica" PUBLIC
        Source/
        Source/Core
        ${Vulkan_INCLUDE_DIRS}
)
target_link_libraries("Unica"
        glfw
        shaderc
        fmt::fmt
        glm::glm
        ${Vulkan_LIBRARIES}
)

set_target_properties(fmt PROPERTIES FOLDER "Libraries")
set_target_properties(glfw update_mappings PROPERTIES FOLDER "Libraries/glfw")
set_target_properties(add-copyright build-version check-copyright shaderc_combined-pkg-config
    shaderc_static-pkg-config shaderc-pkg-config glslc glslc_exe shaderc testdata shaderc-online-compile
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
