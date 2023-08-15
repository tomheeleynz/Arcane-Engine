cmake_minimum_required(VERSION 3.17)
project(ArcaneRuntime)

file(GLOB SRC_FILES "src/*.h" "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} Arcane)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Tools")