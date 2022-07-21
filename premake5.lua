require("cmake")
workspace "Unica"
    architecture "x64"
    startproject "Unica"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Unica"
