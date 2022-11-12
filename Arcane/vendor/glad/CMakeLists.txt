cmake_minimum_required(VERSION 3.17)
project(glad)

file(GLOB SRC_FILES "src/*.c")
file(GLOB GLAD_FILES "include/glad/glad.h")
file(GLOB KHR_FILES "include/KHR/khrplatform.h")

add_library(glad STATIC ${SRC_FILES} ${GLAD_FILES} ${KHR_FILES})

target_include_directories(glad PUBLIC "include")
