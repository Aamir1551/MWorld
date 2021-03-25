cmake_minimum_required(VERSION ${using_cmake_version})
project(render)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(camera)
add_subdirectory(cube_renderer)
add_subdirectory(utils)
add_subdirectory(shader)
add_subdirectory(block_renderer)
