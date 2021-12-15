workspace "Unica"
    architecture "x64"
    startproject "Unica"

    configurations
    {
        "Debug",
        "Shipping"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Unica/Libraries/glfw"
group ""

include "Unica"
