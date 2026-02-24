workspace "Pixellia"
    configurations { "Debug", "Release" }
    architecture "x64"

project "Pixellia"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    architecture "x64"
    
    targetdir ("bin/" .. "%{cfg.buildcfg}" .. "/%{prj.name}")
    objdir ("bin-int/" .. "%{cfg.buildcfg}" .. "/%{prj.name}")

    files
    {
        "Excellia/src/**.cpp",
        "Excellia/include/**.h",
        "Excellia/include/**.hpp"
    }

    includedirs
    {
        "Excellia/include",
        "vendor/sfml/include"
    }

    libdirs
    {
        "vendor/sfml/lib"
    }

    links
    {
        "sfml-graphics",
        "sfml-window",
        "sfml-system",
        "sfml-audio",
        "sfml-network",
        "opengl32",
        "winmm",
        "gdi32"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "DEBUG", "_DEBUG" }
        symbols "On"
        runtime "Debug"
        links
        {
            "sfml-graphics-d",
            "sfml-window-d",
            "sfml-system-d",
            "sfml-audio-d",
            "sfml-network-d"
        }
        removelinks
        {
            "sfml-graphics",
            "sfml-window",
            "sfml-system",
            "sfml-audio",
            "sfml-network"
        }
        postbuildcommands
        {
            '{COPY} "%{wks.location}vendor/sfml/bin/sfml-graphics-d-2.dll" "%{cfg.targetdir}"',
            '{COPY} "%{wks.location}vendor/sfml/bin/sfml-window-d-2.dll" "%{cfg.targetdir}"',
            '{COPY} "%{wks.location}vendor/sfml/bin/sfml-system-d-2.dll" "%{cfg.targetdir}"',
            '{COPY} "%{wks.location}vendor/sfml/bin/sfml-audio-d-2.dll" "%{cfg.targetdir}"',
            '{COPY} "%{wks.location}vendor/sfml/bin/sfml-network-d-2.dll" "%{cfg.targetdir}"'
        }

    filter "configurations:Release"
        defines { "NDEBUG", "RELEASE" }
        optimize "On"
        runtime "Release"
        postbuildcommands
        {
            '{COPY} "%{wks.location}vendor/sfml/bin/sfml-graphics-2.dll" "%{cfg.targetdir}"',
            '{COPY} "%{wks.location}vendor/sfml/bin/sfml-window-2.dll" "%{cfg.targetdir}"',
            '{COPY} "%{wks.location}vendor/sfml/bin/sfml-system-2.dll" "%{cfg.targetdir}"',
            '{COPY} "%{wks.location}vendor/sfml/bin/sfml-audio-2.dll" "%{cfg.targetdir}"',
            '{COPY} "%{wks.location}vendor/sfml/bin/sfml-network-2.dll" "%{cfg.targetdir}"'
        }

    filter {}