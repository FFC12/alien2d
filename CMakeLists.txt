cmake_minimum_required(VERSION 3.0)
project(GameFramework)

set(CMAKE_CXX_STANDARD 20)

include_directories(alien2d)
add_executable(GameFramework main.cpp alien2d/common.hpp)

find_package(OpenGL REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenGL_LIBRARIES})
