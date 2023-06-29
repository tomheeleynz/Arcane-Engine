cmake_minimum_required(VERSION 3.17)
project(ArcaneEngine)

# Arcane Engine Library
add_subdirectory(Arcane)

# Kinetics Physicss Library
add_subdirectory(Kinetics)

# EnchantingTable Editor
add_subdirectory(EnchantingTable)

# Vendor Libraries
# -- GLFW 
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("Arcane/vendor/glfw")

# -- ImGui
add_subdirectory("Arcane/vendor/imgui")

# -- Assimp
add_subdirectory("Arcane/vendor/assimp")

# -- Glad
add_subdirectory("Arcane/vendor/glad")

# -- SPIRV-Cross
add_subdirectory("Arcane/vendor/SPIRV-Cross")

# -- Set Global Properties
set_property(TARGET Arcane PROPERTY CXX_STANDARD 17)
set_property(TARGET Kinetics PROPERTY CXX_STANDARD 17)
set_property(TARGET EnchantingTable PROPERTY CXX_STANDARD 17)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)