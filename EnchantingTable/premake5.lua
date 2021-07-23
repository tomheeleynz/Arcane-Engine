project "EnchantingTable"
    kind "ConsoleApp"
    cppdialect "C++17"
    language "C++"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs {
        "src",
        "%{wks.location}/Arcane/src",
        "%{IncludeDirs.vulkan}",
        "%{IncludeDirs.glm}",
        "%{IncludeDirs.glfw}",
        "%{IncludeDirs.imgui}"
    }

    links {
        "Arcane"
    }

    filter "system:windows"
        systemversion "latest"
