project "Arcane"
    kind "StaticLib"
    cppdialect "C++17"
    language "C++"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp"
    }

    
    includedirs {
        "%{IncludeDirs.glfw}",
        "%{IncludeDirs.vulkan}",
        "%{IncludeDirs.glm}",
        "%{IncludeDirs.imgui}",
        "src/Arcane"
    }

    links {
        "glfw",
        "imgui",
        "%{LibDirs.vulkan}/vulkan-1.lib"
    }

    filter "system:windows"
        systemversion "latest"
