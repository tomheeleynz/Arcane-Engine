cmake_minimum_required(VERSION 3.17)
project(Arcane)

# Set File Stuff
file(GLOB_RECURSE SRC_FILES "src/*.h" "src/*.cpp")

# Find Vulkan
find_package(Vulkan REQUIRED)

# Set Libraries
set(LIBS glfw imgui ${Vulkan_LIBRARIES})

# Create Library
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

# Link Source Directories
target_include_directories(${PROJECT_NAME} PUBLIC "src" "vendor/glm" ${Vulkan_INCLUDE_DIRS} "vendor/stb")

# Link Libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})