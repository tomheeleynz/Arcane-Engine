cmake_minimum_required(VERSION 3.17)
project(Kinetics)

file(GLOB CORE_FILES "src/Kinetics/Core/*.cpp"  "include/Kinetics/Core/*.h")
file(GLOB MATHS_FILES "include/Kinetics/Maths/*.h")
file(GLOB STRUCTURES_FILES "src/Kinetics/Structures/*.cpp" "include/Kinetics/Structures/*.h")


add_library(${PROJECT_NAME} STATIC ${CORE_FILES} ${MATHS_FILES} ${STRUCTURES_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC "include")

# Set Folder
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Core")