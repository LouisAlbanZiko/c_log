workspace "Sandbox"

	configurations { "Debug", "Release" }
	architecture "x64"

	outputpath = "%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}/"

	targetdir ("_bin/%{prj.name}/" .. outputpath)
	objdir ("_bin_obj/%{prj.name}/" .. outputpath)

	project "Sandbox"
		location "Sandbox"
		kind "ConsoleApp"
		language "C"
		
		files { "Sandbox/**.c" }
		includedirs { "_vendor", "Sandbox" }

		includedirs { "src" }
		links { "c_log" }

	include "src"