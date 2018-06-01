-- premake5.lua
workspace "ecs_engine"
	location "Build/"
	platforms { "x64" }
	configurations { "Debug", "Release", "DebugServer", "ReleaseServer" }
	targetdir "Bin/%{prj.name}"
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
		"Source/**.h", "Source/**.hpp",
		"Source/**.cpp", "Source/**.c", "Source/**.inl",
		"Build/gen/**.h", "Build/gen/**.cpp",

		-- Resource Files
		"Resource/**.*"
	}

	vpaths {
		[".generated"] = "**/gen/*",
		["Code/*"] = { "Source/*" },
		["Resource/*"] = { "Resource/* "}
	}

	-- systemversion "10.0.16299.0"

	objdir "Build/obj/"
	targetsuffix "_%{cfg.buildcfg}"

	pchheader "CorePCH.h"
	pchsource "Source/CorePCH.cpp"

	includedirs { "Dependency/include", "Source", "Build/gen" }

	prebuildcommands {
		"\"%{wks.location}..\\Bin\\CodeGen\\CodeGen.exe\" \"%{wks.location}..\\Source\" \"%{wks.location}gen\""
	}

	postbuildcommands {
		"{COPY} \"../Dependency/dll/%{cfg.buildcfg}/*\" %{cfg.buildtarget.directory}"
	}
	
	links { "angelscript64" }
 	
	filter "Release or ReleaseServer"
		defines { "NDEBUG" }
		optimize "On"
		libdirs "Dependency/lib/Release"

	filter "Debug or DebugServer"
		defines { "_DEBUG", "DEBUG" }
		symbols "On"
		libdirs "Dependency/lib/Debug"
		
	filter "Debug"
		links { "glew32sd" }

	filter "Release"
		links { "glew32s" }

	filter "Debug or Release"
		defines { "GLEW_STATIC" }
		links { "opengl32", "glfw3", "SOIL", "assimp.lib" }

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

		objdir "Tools/codegen/Build/obj"

		files { "Tools/codegen/Source/**.cs" }
		vpaths { [""] = "**.cs" }

		debugargs { "\"%{wks.location}..\\Source\" \"%{wks.location}gen\"" }

		links { "System" }

		filter "Debug"
			defines { "_DEBUG", "DEBUG" }
