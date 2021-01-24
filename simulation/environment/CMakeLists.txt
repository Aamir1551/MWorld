cmake_minimum_required(VERSION 3.10)
project(simulation)
set(CXX_STANDARD 11)

add_executable(simulation simulation.cpp)
target_link_libraries(simulation PUBLIC linear_alg quaternion cube_renderer utils glad ${glfw} settings  mworld_render_utils camera shader blocks)

target_include_directories(simulation PUBLIC "${glfw_include_path}"  "${glm_include_path}" "${CMAKE_CURRENT_SOURCE_DIR}/" )
