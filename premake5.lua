workspace "Paintball Splatter" -- Solution
    architecture "x64"

    configurations
    {
        "Debug",   -- Debug available
        "Release"  -- Faster version of debug
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" -- Debug-Windows-x64

project "Paintball Splatter" -- Sandbox
        location "Paintball Splatter"
        kind "ConsoleApp"
        language "C++"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files 
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
        }

        includedirs
        {
            "Paintball Splatter/src",
            "Dependencies/GLEW 2.1.0/include",
            "Dependencies/glm"
        }

        libdirs
        {
            "Dependencies/**"
        }

        links
        {
            "Dependencies/GLEW 2.1.0/lib/Release/x64/glew32s.lib",
            "opengl32.lib"
        }

        --[[postbuildcommands
        {
            ("{COPY} Dependencies/SDL2 2.28.2/lib/x64/SDL2.dll" .. outputdir .. "/%{prj.name}")
        }--]]

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest" -- 10.0 (latest installed version) or 10.0.22621.0

        defines
        {
            "PS_PLATFORM_WINDOWS",
            "GLEW_STATIC"
        }

    filter "configurations:Debug"
        defines "PS_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "PS_RELEASE"
        optimize "On"