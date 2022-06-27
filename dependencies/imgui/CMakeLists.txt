cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE IMGUI___HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")
file(GLOB_RECURSE IMGUI___SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

file(GLOB_RECURSE IMGUI_BACKEND___HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "backends/*.h")
file(GLOB_RECURSE IMGUI_BACKEND___SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "backends/src/*.cpp")

add_library(
  imgui STATIC

  ${IMGUI___HEADERS}
  ${IMGUI___SOURCES}
  ${IMGUI_BACKEND___HEADERS}
  ${IMGUI_BACKEND___SOURCES}
)
target_include_directories(
  imgui

  PRIVATE ${CMAKE_SOURCE_DIR}/dependencies/glad
  PRIVATE ${CMAKE_SOURCE_DIR}/dependencies/glfw/include
)
target_link_libraries(
  imgui

  PRIVATE glad
  PRIVATE glfw
  PRIVATE OpenGL::GL
)
