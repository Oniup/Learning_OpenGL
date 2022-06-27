cmake_minimum_required(VERSION 3.20)

project(learning-computer-graphics VERSION 0.0.1)

# setting up glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_compile_definitions(PRINT_SOURCE_DEBUG)

# locating the OpenGL package to be linked
find_package(OpenGL REQUIRED)

add_subdirectory(dependencies/glad)
add_subdirectory(dependencies/glfw)

add_subdirectory(dependencies/imgui)

file(GLOB_RECURSE APPLICATION_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "app/src/*.cpp")

add_executable(${CMAKE_PROJECT_NAME} ${APPLICATION_SOURCES} main.cpp)

target_include_directories(
  ${CMAKE_PROJECT_NAME}

  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw/include
)

target_link_libraries(
  ${CMAKE_PROJECT_NAME}

  PRIVATE glad
  PRIVATE glfw
  PRIVATE OpenGL::GL
  PRIVATE imgui
)

# copying over all the shaders and textures

add_custom_target(copy_assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/app/assets ${CMAKE_CURRENT_BINARY_DIR}/app/assets
)

add_dependencies(${CMAKE_PROJECT_NAME} copy_assets)