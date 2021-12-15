project "glfw"
    kind "StaticLib"
    language "C"

    targetdir ("../../../Build/" .. outputdir .. "/%{prj.name}")
    objdir ("../../../Intermediate/" .. outputdir .. "%{prj.name}")

    files
    {
        "glfw/include/GLFW/glfw3.h",
        "glfw/include/GLFW/glfw3native.h",
        "glfw/src/context.c",
        "glfw/src/init.c",
        "glfw/src/input.c",
        "glfw/src/monitor.c",
        "glfw/src/vulkan.c",
        "glfw/src/window.c"
    }

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

        files
        {
            "glfw/src/win32_init.c",
            "glfw/src/win32_joystick.c",
            "glfw/src/win32_monitor.c",
            "glfw/src/win32_time.c",
            "glfw/src/win32_thread.c",
            "glfw/src/win32_window.c",
            "glfw/src/wgl_context.c",
            "glfw/src/egl_context.c",
            "glfw/src/osmesa_context.c"
        }

        defines 
        { 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        symbols "on"

    filter "configurations:Shipping"
        optimize "on"