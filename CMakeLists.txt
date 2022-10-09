#WARNING: Here is the thing:
# When you don't use the Visual C++ compiler, it
# never finds it out Dx11 shared library since `pragma`
# is nothing to do with GCC/CLANG compilers.
# So you have to use the Visual C++ for Dx11 for now.
# otherwise for the OpenGL, all is fine.

cmake_minimum_required(VERSION 3.0)
project(GameFramework)

set(CMAKE_CXX_STANDARD 20)

include_directories(alien2d)
add_executable(GameFramework main.cpp alien2d/common.hpp)

find_package(OpenGL REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenGL_LIBRARIES})
