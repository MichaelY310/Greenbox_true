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
    include "Greenbox/Dependencies/GLFW"
    include "Greenbox/Dependencies/Glad"
    include "Greenbox/Dependencies/imgui"
    include "Greenbox/Dependencies/yaml-cpp"
group ""

group "Core"
	include "Greenbox"
group ""

group "Misc"
	include "Sandbox"
group ""