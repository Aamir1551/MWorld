cmake_minimum_required(VERSION 3.10)
project(quaternion_lib)
set(CMAKE_CXX_STANDARD 11)

add_library(octree octree.cpp octree.hpp)
target_include_directories(octree PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.")
target_link_libraries(octree PUBLIC linear_alg settings)