cmake_minimum_required(VERSION 3.17)
project(Arcane)

file(GLOB CORE_FILES "src/Arcane/Core/*.cpp"  "src/Arcane/Core/*.h")
file(GLOB RENDERER_FILES "src/Arcane/Renderer/*.h" "src/Arcane/Renderer/*.cpp")
file(GLOB ECS_FILES "src/Arcane/ECS/*.h" "src/Arcane/ECS/*.cpp")
file(GLOB SCENE_FILES "src/Arcane/Scene/*.h" "src/Arcane/Scene/*.cpp")
file(GLOB UTIL_FILES "src/Arcane/Utils/*.h" "src/Arcane/Utils/*.cpp")
file(GLOB ASSET_MANAGER_FILES "src/Arcane/Assets/*.h" "src/Arcane/Assets/*.cpp")
file(GLOB SCRIPTING_FILES "src/Arcane/Scripting/*.h" "src/Arcane/Scripting/*.cpp")
file(GLOB PROJECT_SYSTEM_FILES "src/Arcane/Project/*.h" "src/Arcane/Project/*.cpp")

if (WIN32 OR (UNIX AND NOT APPLE))
	# Find Vulkan
	find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)

	# Find OpenGL
	find_package(OpenGL REQUIRED)

	# Only add spirv-reflect on windows and linux
	add_subdirectory("vendor/SPIRV-Reflect")

	message(STATUS ${Vulkan_shaderc_combined_LIBRARY})

	# Set Libraries
	set(LIBS glfw imgui ${Vulkan_LIBRARIES} "C:/VulkanSDK/1.3.216.0/Lib/shaderc_combinedd.lib" assimp SPIRV-REFLECT glad ${OPENGL_LIBRARIES} spirv-cross-glsl)

	# Set Vulkan engine files
	file(GLOB VULKAN_FILES "src/Arcane/Platform/Vulkan/*.h" "src/Arcane/Platform/Vulkan/*.cpp")
	file(GLOB OPENGL_FILES "src/Arcane/Platform/OpenGL/*.h" "src/Arcane/Platform/OpenGL/*.cpp" )
	file(GLOB IMGUI_FILES "src/Arcane/ImGui/ImGuiLayer.h" "src/Arcane/ImGui/ImGuiLayer.cpp" "src/Arcane/ImGui/PlatformImGui.h"  "src/Arcane/ImGui/PlatformImGui.cpp")

	if (WIN32)
		add_definitions(-DARCANE_WIN32)
		message(STATUS "Compiling for windows")
		
		file(GLOB WINDOWS_UTILS "src/Arcane/Platform/Windows/*.h"  "src/Arcane/Platform/Windows/*.cpp" )

		set(PLATFORM_FILES ${VULKAN_FILES} ${WINDOWS_UTILS} ${OPENGL_FILES})
	else()
		add_definitions(-DARCANE_LINUX)
		message(STATUS "Compiling for linux")
	endif()
endif()

if (APPLE)
	add_definitions(-DARCANE_APPLE)
	message(STATUS "Compiling for apple")
endif()

# Find Python for scripting
find_package (Python COMPONENTS Interpreter Development)
message(STATUS ${Python_INCLUDE_DIRS})
message(STATUS ${Python_LIBRARIES})
message(STATUS ${Python_LIBRARY_DIRS})

# Add pybind as a subdirectory
add_subdirectory("vendor/pybind11")

# Create Library
add_library(
	${PROJECT_NAME}
	STATIC 
	${CORE_FILES} 
	${RENDERER_FILES} 
	${PLATFORM_FILES} 
	${IMGUI_FILES}
	${ECS_FILES}
	${SCENE_FILES}
	${UTIL_FILES}
	${ASSET_MANAGER_FILES}
	${SCRIPTING_FILES}
	${PROJECT_SYSTEM_FILES}
)

# Link Source Directories
target_include_directories(
	${PROJECT_NAME} PUBLIC 
	"src" 
	"vendor/glm" 
	"vendor/stb" 
	"vendor/entt/single_include" 
	"vendor/json/single_include" 
	"vendor/dotnet" 
	${OPENGL_INCLUDE_DIRS} 
	"vendor/SPIRV-Cross"  
	"vendor/pybind11/include"
	${Python_INCLUDE_DIRS}
)

# Link Libraries
target_link_libraries(${PROJECT_NAME} ${LIBS} ${Python_LIBRARIES})

# Set Folder
set_target_properties(Arcane PROPERTIES FOLDER "Core")