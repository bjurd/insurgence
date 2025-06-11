workspace "Insurgence"
	configurations { "Debug", "Release" }
	platforms { "x64" }
	startproject "Insurgence"

project "Insurgence"
	location "build"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	system "Windows"
	systemversion "latest"
	defines { "WIN32_LEAN_AND_MEAN" }

	files { "src/**.h", "src/**.hpp", "src/**.cpp", "src/**.c" }
	includedirs { "$(DXSDK_DIR)Include" }
	libdirs { "$(DXSDK_DIR)Lib\\x64" }
	links { "d3d9.lib", "d3dx9.lib", "dxerr.lib", "dxguid.lib" }

	filter "configurations:Debug"
		defines { "_DEBUG" }
		symbols "On"
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "Full"
		symbols "Off"
		linktimeoptimization "On"
		buildoptions { "/Gw", "/Gy" }
		linkoptions { "/OPT:REF", "/OPT:ICF" }
	filter {}

	vpaths {
		["Source Files/*"] = { "src/**.cpp", "src/**.c" },
		["Header Files/*"] = { "src/**.h", "src/**.hpp" }
	}
