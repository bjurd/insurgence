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

	files { "insurgence/**.h", "insurgence/**.hpp", "insurgence/**.cpp", "insurgence/**.c" }
	includedirs { "$(DXSDK_DIR)Include" }
	libdirs { "$(DXSDK_DIR)Lib\\x64" }
	links { "d3d9.lib", "d3dx9.lib", "dxerr.lib", "dxguid.lib" }

	filter "configurations:Debug"
		defines { "_DEBUG" }
		symbols "On"
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
	filter {}

	vpaths {
		["Source Files/*"] = { "insurgence/**.cpp", "insurgence/**.c" },
		["Header Files/*"] = { "insurgence/**.h", "insurgence/**.hpp" }
	}
