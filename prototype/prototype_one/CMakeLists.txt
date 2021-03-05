cmake_minimum_required(VERSION 3.10)
project(prototype_one)

set(CMAKE_CXX_STANDARD 11)
set(current_target_name prototype_one)

add_executable(${current_target_name} main.cpp)

#target_include_directories(${current_target_name} PUBLIC "${glfw_include_path}" "${glm_include_path}")
target_link_libraries(${current_target_name} PUBLIC utils glad "${glfw}" cube_renderer  mworld_render_utils camera shader)
target_link_libraries(${current_target_name} PUBLIC utils glad cube_renderer  mworld_render_utils camera shader)
