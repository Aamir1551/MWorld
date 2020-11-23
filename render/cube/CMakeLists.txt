cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

project(cube)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../numerics/linear_alg" "./linear_alg")
add_library(cube "${CMAKE_CURRENT_SOURCE_DIR}/cube.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/cube.cpp")

target_include_directories(cube PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../numerics/linear_alg/include/")
target_link_libraries(cube linear_alg)