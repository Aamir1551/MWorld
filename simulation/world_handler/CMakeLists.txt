cmake_minimum_required(VERSION 3.10)
project(world_handler)
set(CXX_STANDARD 11)

add_library(world_handler world_handler.cpp world_handler.hpp)
target_link_libraries(world_handler PUBLIC octree force_octree linear_alg quaternion utils settings blocks)
target_include_directories(world_handler PUBLIC "${glfw_include_path}"  "${glm_include_path}" "${CMAKE_CURRENT_SOURCE_DIR}/" )
