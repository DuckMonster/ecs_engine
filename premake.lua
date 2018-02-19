-- premake5.lua
workspace "ecs_engine"
	location "build/"
	platforms { "x64" }
	configurations { "Debug", "Release", "DebugServer", "ReleaseServer" }
	targetdir "bin/%{prj.name}"
	startproject "Game"

project "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++11"
	architecture "x64"

	debugdir "."

	dependson "CodeGen"

	filter "system:windows"
		defines { "WIN32" ,"_WIN32" }

	files { 
		-- Source files
		"src/**.h", "src/**.hpp",
		"src/**.cpp", "src/**.c", "src/**.inl",
		"obj/gen/**.h", "obj/gen/**.cpp",

		-- Resource Files
		"Resource/**.*"
	}

	vpaths {
		[".generated"] = "**/gen/*",
		["Code/*"] = { "src/*" },
		["Resource/*"] = { "Resource/* "}
	}

	systemversion "10.0.16299.0"

	objdir "obj/"
	targetsuffix "_%{cfg.buildcfg}"

	pchheader "CorePCH.h"
	pchsource "src/CorePCH.cpp"

	includedirs { "include", "D:/Programming/C++/!Common", "src" }

	prebuildcommands {
		"%{wks.location}..\\bin\\CodeGen\\CodeGen.exe %{wks.location}..\\src\\ %{wks.location}..\\obj\\gen\\"
	}
 	
	filter "Release or ReleaseServer"
		defines { "NDEBUG" }
		optimize "On"
		libdirs "lib/Release"

	filter "Debug or DebugServer"
		defines { "_DEBUG", "DEBUG" }
		symbols "On"
		libdirs "lib/Debug"
		
	filter "Debug"
		links { "glew32sd" }

	filter "Release"
		links { "glew32s" }

	filter "Debug or Release"
		defines { "GLEW_STATIC" }
		links { "opengl32", "glfw3", "SOIL" }

	filter "DebugServer or ReleaseServer"
		defines { "GAME_HEADLESS" }

group "Tools"
	project "CodeGen"
		configmap {
			["DebugServer"] = "Debug",
			["ReleaseServer"] = "Release"
		}

		kind "ConsoleApp"
		language "C#"
		location "build/"

		files { "tools/codegen/src/**.cs" }
		vpaths { [""] = "**.cs" }

		debugargs { "D:\\Programming\\C++\\DickEngine\\src D:\\Programming\\C++\\DickEngine\\obj\\gen" }

		links { "System" }

		filter "Debug"
			defines { "_DEBUG", "DEBUG" }