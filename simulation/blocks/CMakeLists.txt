cmake_minimum_required(VERSION ${using_cmake_version})
project(Blocks)
set(CMAKE_CXX_STANDARD 11)

add_library(blocks cube.hpp cube.cpp block.hpp e_block.hpp i_block.hpp m_block.hpp z_block.hpp block.cpp e_block.cpp i_block.cpp m_block.cpp z_block.cpp)
#target_link_libraries(blocks PUBLIC linear_alg quaternion cube_renderer utils glad ${glfw} settings  mworld_render_utils camera shader)
target_link_libraries(blocks PUBLIC linear_alg quaternion  utils  ${glfw} settings)

target_include_directories(blocks PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/" )
target_include_directories(blocks PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../octree/" )

if(compile_with_openmp)
    target_compile_definitions(blocks PUBLIC OPENMP=1)
endif()

