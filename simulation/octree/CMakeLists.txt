cmake_minimum_required(VERSION ${using_cmake_version})
project(quaternion_lib)
set(CMAKE_CXX_STANDARD 11)

add_library(octree octree.cpp octree.hpp)
add_library(collision_octree collision_octree.cpp collision_octree.hpp)
add_library(force_octree force_octree.cpp force_octree.hpp)

target_link_libraries(octree PUBLIC linear_alg settings blocks)
target_link_libraries(collision_octree PUBLIC linear_alg settings blocks octree)
target_link_libraries(force_octree PUBLIC linear_alg settings blocks octree)

target_include_directories(collision_octree PUBLIC ".")
target_include_directories(force_octree PUBLIC ".")
target_include_directories(octree PUBLIC ".")
