cmake_minimum_required(VERSION 3.10)
project(Blocks)
set(CMAKE_CXX_STANDARD 11)

add_library(blocks cube.cpp block.cpp e_block.cpp i_block.cpp m_block.cpp z_block.cpp)
target_link_libraries(blocks PUBLIC linear_alg quaternion cube_renderer utils glad ${glfw} settings  mworld_render_utils camera shader)

target_include_directories(blocks PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/" )
