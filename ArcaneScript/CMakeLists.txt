cmake_minimum_required(VERSION 3.17)
project(ArcaneScript)

file(GLOB TOKEN_SRC_FILES "src/ArcaneScript/Token/*.h" "src/ArcaneScript/Token/*.cpp")

add_library(${PROJECT_NAME} STATIC ${TOKEN_SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC "src")

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Core")