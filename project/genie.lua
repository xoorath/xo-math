solution "xo-math"
	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }

project "demo"
	language "C++"
	userincludedirs { "./include" }
	files { "main.cpp", "source/*.cpp", "include/*.h" }
	kind "ConsoleApp"
	flags { "EnableSSE", "FloatFast", "OptimizeSpeed", "NoExceptions", "ExtraWarnings" }
	defines { "_HAS_EXCEPTIONS=0" }
	configuration "Debug"
		defines { "DEBUG" }
	configuration "Release"
		defines { "RELEASE" }