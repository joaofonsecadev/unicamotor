require("cmake")
workspace "Unica"
    architecture "x64"
    startproject "Unica"

    configurations
    {
        "Debug",
        "Shipping"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Unica"
