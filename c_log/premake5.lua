project "c_log"
	location "."
	kind "StaticLib"
	language "C"
	
	files { "**.c" }
	includedirs { "_vendor", "." }