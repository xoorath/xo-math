function projectDefaults()
    language "C++"
    flags { "NoExceptions", "NoExceptions", "NoRTTI", "FloatFast" }
    configuration "Debug"
        defines { "DEBUG" }
        flags { "Symbols" }
    configuration "Release"
        defines { "Release" }
        defines { "Optimize" }
    configuration { "x32" }
        targetdir("../" .. _ACTION .. "/x32")
    configuration { "x64" }
        targetdir("../" .. _ACTION .. "/x64")
    configuration { }
end

function demoDefaults()
    includedirs {
        "../demos/SDL2-2.0.8/include",
        "../demos/glew-2.1.0/include",
        "../demos/imgui",
        "../xo-math"}
    links {
        "SDL2",
        "SDL2main",
        "glew32",
        "opengl32"
    }
    flags { "WinMain" }
    configuration "x32"
        libdirs { 
            "../demos/SDL2-2.0.8/lib/x86",
            "../demos/glew-2.1.0/lib/Release/Win32"
        }
    configuration "x64"
        libdirs { 
            "../demos/SDL2-2.0.8/lib/x64",
            "../demos/glew-2.1.0/lib/Release/x64"
        }
    configuration { }
end

function setupDemos(...)
    for _, name in ipairs({...}) do
        project(name)
            kind "WindowedApp"
            language "C++"
            files { 
                "../demos/demo-00-common/**.cpp",
                "../demos/demo-00-common/**.h",
                "../demos/" .. name .."/**.cpp", 
                "../demos/" .. name .. "/**.h",
                "../demos/imgui/**.cpp",
                "../demos/imgui/**.h"
            }
            demoDefaults()
            projectDefaults()
    end
end

solution "xo-math"
    configurations {"Debug", "Release" }
    platforms { "x32", "x64" }
    location("../" .. _ACTION)

    project "xo-math"
        kind "ConsoleApp"
        files { "../source/**.h", "../source/**.cpp" }
        targetname "xomath"
        projectDefaults()

    setupDemos("demo-01-cubes")