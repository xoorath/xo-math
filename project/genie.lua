solution "xo-math"
	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }

project "demo"
	language "C++"
	userincludedirs { "./include" }
	files { "main.cpp", "source/*.cpp", "include/*.h" }
	kind "ConsoleApp"
	flags { "EnableAVX2", "FloatFast", "OptimizeSpeed", "NoExceptions", "ExtraWarnings" }
	defines { "_HAS_EXCEPTIONS=0" }
	buildoptions "-std=c++11"
	configuration "Debug"
		defines { "DEBUG" }
	configuration "Release"
		defines { "RELEASE" }

project "tests"
	language "C++"
	userincludedirs { "./include" }
	files { "tests/*.cpp", "tests/*.h", "source/*.cpp", "include/*.h" }
	kind "ConsoleApp"
	flags { "EnableAVX2", "FloatFast", "OptimizeSpeed", "NoExceptions", "ExtraWarnings" }
	defines { "_HAS_EXCEPTIONS=0" }
	buildoptions "-std=c++11"
	configuration "Debug"
		defines { "DEBUG" }
	configuration "Release"
		defines { "RELEASE" }