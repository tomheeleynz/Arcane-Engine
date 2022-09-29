cmake_minimum_required(VERSION 3.17)
project(Arcane-ScriptCore LANGUAGES CSharp)

file(GLOB ECS_FILES "src/Arcane/ECS/*.cs")
file(GLOB MATHS_FILES "src/Arcane/Maths/*.cs")


add_library(${PROJECT_NAME} SHARED ${ECS_FILES} ${MATHS_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Core")