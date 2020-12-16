cmake_minimum_required(VERSION 3.10)
project(prototype_three)
set(CXX_STANDARD 11)

add_executable(prototype_three cube.cpp main.cpp)
target_link_libraries(prototype_three PUBLIC linear_alg quaternion cube_renderer utils glad ${glfw} settings  mworld_render_utils camera shader)

target_include_directories(prototype_three PUBLIC "${glfw_include_path}"  "${glm_include_path}" "${CMAKE_CURRENT_SOURCE_DIR}/" )