workspace "ArcaneEngine"
    configurations { "Debug", "Release" }
    architecture "x86_64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Get Vulkan SDK Path
VULKAN_SDK = os.getenv("VULKAN_SDK")

-- Include Dirs
IncludeDirs = {}
IncludeDirs["glfw"] = "%{wks.location}/Arcane/vendor/glfw/include"
IncludeDirs["vulkan"] = "%{VULKAN_SDK}/Include"
IncludeDirs["glm"] ="%{wks.location}/Arcane/vendor/glm"
IncludeDirs["imgui"] = "%{wks.location}/Arcane/vendor/imgui"
IncludeDirs["stb"] =  "%{wks.location}/Arcane/vendor/stb"

-- Library Includes
LibDirs = {}
LibDirs["vulkan"] = "%{VULKAN_SDK}/Lib"

-- Core Apps
group "Core"
    include "Arcane"
group ""
    
group "Tools"
	include "EnchantingTable"	
group ""

-- Dependencies Group
group "Dependencies"
    include "Arcane/vendor/glfw"
    include "Arcane/vendor/imgui"
group ""