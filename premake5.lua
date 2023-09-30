include "Dependencies.lua"

workspace "Greenbox"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "GreenBox/Dependencies/GLFW"
    include "GreenBox/Dependencies/Glad"
    include "GreenBox/Dependencies/imgui"
    include "GreenBox/Dependencies/yaml-cpp"
group ""

include "Greenbox"
include "Sandbox"