cmake_minimum_required(VERSION 3.10)
project(opengl)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES "*.cpp")

add_executable(opengl ${SOURCES})

# Use the package PkgConfig to detect and setup GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(opengl ${GLFW_LIBRARIES})

# Use the package PkgConfig to detect and setup FREEGLUT
find_package(PkgConfig REQUIRED)
pkg_search_module(FREEGLUT REQUIRED freeglut)

include_directories(${FREEGLUT_INCLUDE_DIRS})
target_link_libraries(opengl ${FREEGLUT_LIBRARIES})

# Use the package PkgConfig to detect and setup GLEW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLEW REQUIRED glew)

include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(opengl ${GLEW_LIBRARIES})

# Additional include paths for OpenGL headers
include_directories("path/to/opengl/headers")

# Link the opengl32 library
target_link_libraries(opengl opengl32)
