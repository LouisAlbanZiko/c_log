project "c_log"
	location "."
	kind "StaticLib"
	language "C"
	
	files { "**.c", "**.h" }
	includedirs { "../_vendor", ".", ".." }

	filter "system:linux"
		links { "m" }
		buildoptions "-g"