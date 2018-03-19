solution "xo-math"
    configurations {"Debug", "Release"}
    location("../" .. _ACTION)

    project "xo-math"
        kind "ConsoleApp"
        language "C++"
        files { "../source/**.h", "../source/**.cpp" }
        targetdir("../" .. _ACTION .. "/build")
        targetname "xomath"

        flags { "NoExceptions", "NoExceptions", "NoRTTI", "FloatFast" }

        configuration "Debug"
            defines { "DEBUG" }
            flags { "Symbols" }
        
        configuration "Release"
            defines { "Release" }
            defines { "Optimize" }