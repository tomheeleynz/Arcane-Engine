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
file(GLOB PHYSICS_FILES "src/Arcane/Physics/*.h" "src/Arcane/Physics/*.cpp")

if (WIN32 OR (UNIX AND NOT APPLE))
	# Find Vulkan
	find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)

	# Find OpenGL
	find_package(OpenGL REQUIRED)

	# Only add spirv-reflect on windows and linux
	add_subdirectory("vendor/SPIRV-Reflect")

	# Shaderc
	message(STATUS "Shaderc Lib: ${Vulkan_shaderc_combined_LIBRARY}")

	# Get the parent directory of the shaderc libraries (This might be different depending)
	get_filename_component(SHADERC_PARENT_DIR ${Vulkan_shaderc_combined_LIBRARY} DIRECTORY)
	set(SHADERC_DEBUG_LIB  "${SHADERC_PARENT_DIR}/shaderc_combinedd.lib")
	set(SHADERC_RELEASE_LIB "${SHADERC_PARENT_DIR}/shaderc_combined.lib")
	
	# Set Physx Library Dir Location
	set(PHYSX_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/PhysX/physx/bin/win.x86_64.vc143.md/debug")
	set(PHYSX_RELEASE_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/PhysX/physx/bin/win.x86_64.vc143.mt/release")

	# Get all the Physx Libraries I need
	set(PHYSX_LIBS ${PHYSX_LIB_DIR}/PhysX_64.lib ${PHYSX_LIB_DIR}/PhysXCommon_64.lib ${PHYSX_LIB_DIR}/PhysXCooking_64.lib ${PHYSX_LIB_DIR}/PhysXFoundation_64.lib ${PHYSX_LIB_DIR}/PhysXExtensions_static_64.lib)

	# Get Physx Release Libs
	set(PHYSX_RELEASE_LIBS ${PHYSX_RELEASE_LIB_DIR}/PhysX_64.lib ${PHYSX_RELEASE_LIB_DIR}/PhysXCommon_64.lib 
		${PHYSX_RELEASE_LIB_DIR}/PhysXCooking_64.lib ${PHYSX_RELEASE_LIB_DIR}/PhysXFoundation_64.lib ${PHYSX_RELEASE_LIB_DIR}/PhysXExtensions_static_64.lib)
	
	# Set Libraries
	set(LIBS glfw imgui ${Vulkan_LIBRARIES} assimp SPIRV-REFLECT glad ${OPENGL_LIBRARIES} spirv-cross-glsl)
	
	# set(LIBS glfw imgui ${Vulkan_LIBRARIES} "C:/VulkanSDK/1.3.216.0/Lib/shaderc_combinedd.lib" assimp SPIRV-REFLECT glad ${OPENGL_LIBRARIES} spirv-cross-glsl)

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

# Try Find Lua
list(APPEND CMAKE_PREFIX_PATH "vendor/lua")
find_package(Lua REQUIRED)

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
	${PHYSICS_FILES}
)

# Link Source Directories
target_include_directories(
	${PROJECT_NAME} 
	PUBLIC 
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
	"vendor/PhysX/physx/include"
	${LUA_INCLUDE_DIR}
)

# Link Libraries
target_link_libraries(
	${PROJECT_NAME} ${LIBS} ${SHADERC_DEBUG_LIB} ${PHYSX_LIBS} ${LUA_LIBRARIES} 
)

# Set Folder
set_target_properties(Arcane PROPERTIES FOLDER "Core")