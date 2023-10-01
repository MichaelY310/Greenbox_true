project "Greenbox"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gbpch.h"
	pchsource "src/PCH/gbpch.cpp"

	files
    {
        "include/**.h",
        "include/**.cpp",
        "src/**.h",
        "src/**.cpp",
        "Dependencies/stb_image/**.h",
        "Dependencies/stb_image/**.cpp",
        "Dependencies/glm/glm/**.hpp",
        "Dependencies/glm/glm/**.inl",

        "Dependencies/ImGuizmo/ImGuizmo.h",
        "Dependencies/ImGuizmo/ImGuizmo.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs 
    {
        "src",
        "include",
        "Dependencies/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}", 
        "%{IncludeDir.glm}", 
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.spdlog}",
    } 

    links 
    { 
        "GLFW", 
        "Glad", 
        "ImGui",
        "yaml-cpp",
        "opengl32.lib"
    }

    filter "files:Dependencies/ImGuizmo/**.cpp"
    flags { "NoPCH" }

    filter "system:window"
        systemversion "latest"

        defines
        {
            "GB_PLATFORM_WINDOWS",
            "GB_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
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