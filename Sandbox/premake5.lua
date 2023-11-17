project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Greenbox/Dependencies/spdlog/include",
		"%{wks.location}/Greenbox/src",
		"%{wks.location}/Greenbox/include",
		"%{wks.location}/Greenbox/Dependencies",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}", 
        "%{IncludeDir.glm}", 
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.Box2D}",

	}

	links 
    {
        "Greenbox"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "GB_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE"

        }

    filter "configurations:Debug"
        defines "GB_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "GB_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "GB_DIST"
        runtime "Release"
        optimize "on"