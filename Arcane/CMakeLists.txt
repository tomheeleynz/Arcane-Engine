cmake_minimum_required(VERSION 3.17)
project(Arcane)


file(GLOB CORE_FILES "src/Arcane/Core/*.cpp"  "src/Arcane/Core/*.h")
file(GLOB RENDERER_FILES "src/Arcane/Renderer/*.h" "src/Arcane/Renderer/*.cpp")

if (WIN32 OR (UNIX AND NOT APPLE))
	# Find Vulkan
	find_package(Vulkan REQUIRED)

	# Set Libraries
	set(LIBS glfw imgui ${Vulkan_LIBRARIES})

	# Set Vulkan engine files
	file(GLOB VULKAN_FILES "src/Arcane/Platform/Vulkan/*.h" "src/Arcane/Platform/Vulkan/*.cpp")
	file(GLOB IMGUI_FILES "src/Arcane/ImGui/ImGuiLayer.h" "src/Arcane/ImGui/ImGuiLayer.cpp" "src/Arcane/ImGui/PlatformImGui.h"  "src/Arcane/ImGui/PlatformImGui.cpp")

	if (WIN32)
		add_compile_definitions("ARCANE_WIN32")
		message(STATUS "Compiling for windows")
		
		set(PLATFORM_FILES ${VULKAN_FILES})
	else()
		add_compile_definitions("ARCANE_UNIX")
		message(STATUS "Compiling for linux")
	endif()
endif()

if (APPLE)
	add_compile_definitions("ARCANE_APPLE")
	message(STATUS "Compiling for apple")

	set(LIBS glfw imgui "-framework Metal" "-framework MetalKit" "-framework Cocoa")
	file(GLOB IMGUI_FILES "src/Arcane/ImGui/ImGuiLayer.h" "src/Arcane/ImGui/ImGuiLayer.cpp")
endif()

# Create Library
add_library(${PROJECT_NAME} STATIC ${CORE_FILES} ${RENDERER_FILES} ${PLATFORM_FILES} ${IMGUI_FILES})

# Link Source Directories
target_include_directories(${PROJECT_NAME} PUBLIC "src" "vendor/glm" "vendor/stb" )

# Link Libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})