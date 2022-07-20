cmake_minimum_required(VERSION 3.20)

add_library(
    glad STATIC

    ${CMAKE_CURRENT_SOURCE_DIR}/glad/glad.h
    ${CMAKE_CURRENT_SOURCE_DIR}/glad.c
)

target_include_directories(
    glad 

    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)