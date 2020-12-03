cmake_minimum_required(VERSION 3.10)
project(render)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(camera)
add_subdirectory(cube_renderer)
add_subdirectory(utils)
add_subdirectory(shader)
