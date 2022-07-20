project "Unica"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("../Build/" .. outputdir .. "/%{prj.name}")
    objdir ("../Intermediate/" .. outputdir .. "%{prj.name}")
    
    files
    {
        "Config/**.ini",
        "Source/**.h",
        "Source/**.cpp"
    }
    
    includedirs
    {
        "Source",
    }

filter "configurations:Debug"
    defines "UNICA_DEBUG"
    symbols "on"

filter "configurations:Shipping"
    defines "UNICA_SHIPPING"
    optimize "on"