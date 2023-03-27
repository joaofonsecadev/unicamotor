cmake_minimum_required(VERSION 3.21)

set(SourceFiles
    Config/BaseEngine.ini
    Shaders/shader.frag
    Shaders/shader.vert
    Source/Core/UnicaFileUtilities.cpp
    Source/Core/UnicaFileUtilities.h
    Source/Core/UnicaInstance.cpp
    Source/Core/UnicaInstance.h
    Source/Core/UnicaMinimal.h
    Source/Core/UnicaSettings.h
    Source/Log/Logger.cpp
    Source/Log/Logger.h
    Source/Main.cpp
    Source/Renderer/RenderInterface.h
    Source/Renderer/RenderManager.cpp
    Source/Renderer/RenderManager.h
    Source/Renderer/Vulkan/Shaders/ShaderUtilities.cpp
    Source/Renderer/Vulkan/Shaders/ShaderUtilities.h
    Source/Renderer/Vulkan/VulkanAPI.cpp
    Source/Renderer/Vulkan/VulkanAPI.h
    Source/Renderer/Vulkan/VulkanQueueFamilyIndices.cpp
    Source/Renderer/Vulkan/VulkanQueueFamilyIndices.h
    Source/Renderer/Vulkan/VulkanRenderWindow.cpp
    Source/Renderer/Vulkan/VulkanRenderWindow.h
    Source/Renderer/Vulkan/VulkanSwapChainSupportDetails.h
    Source/Renderer/Vulkan/VulkanTypeInterface.h
    Source/Renderer/Vulkan/VulkanTypes/VulkanImageView.cpp
    Source/Renderer/Vulkan/VulkanTypes/VulkanImageView.h
    Source/Renderer/Vulkan/VulkanTypes/VulkanInstance.cpp
    Source/Renderer/Vulkan/VulkanTypes/VulkanInstance.h
    Source/Renderer/Vulkan/VulkanTypes/VulkanLogicalDevice.cpp
    Source/Renderer/Vulkan/VulkanTypes/VulkanLogicalDevice.h
    Source/Renderer/Vulkan/VulkanTypes/VulkanPhysicalDevice.cpp
    Source/Renderer/Vulkan/VulkanTypes/VulkanPhysicalDevice.h
    Source/Renderer/Vulkan/VulkanTypes/VulkanSwapChain.cpp
    Source/Renderer/Vulkan/VulkanTypes/VulkanSwapChain.h
    Source/Renderer/Vulkan/VulkanTypes/VulkanWindowSurface.cpp
    Source/Renderer/Vulkan/VulkanTypes/VulkanWindowSurface.h
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

find_package(Vulkan REQUIRED FATAL_ERROR)

target_include_directories("Unica" PUBLIC
        Source/
        Source/Core
        ${Vulkan_INCLUDE_DIRS}
)
target_link_libraries("Unica"
        fmt::fmt
        glm::glm
        SDL3::SDL3
        spdlog::spdlog
        Tracy::TracyClient
        ${Vulkan_LIBRARIES}
)