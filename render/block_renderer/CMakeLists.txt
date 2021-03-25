cmake_minimum_required(VERSION ${using_cmake_version})
project(block_renderer CXX)
set(CMAKE_CXX_STANDARD 11)
set(project_name block_renderer)

add_library(block_renderer "${CMAKE_CURRENT_SOURCE_DIR}/block_renderer.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/block_renderer.cpp" )

target_include_directories(block_renderer PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/../lib/" "${glfw_include_path}" "${glm_include_path}" "${CMAKE_CURRENT_SOURCE_DIR}/.")
target_link_libraries(block_renderer PUBLIC linear_alg quaternion cube_renderer utils glad ${glfw} settings  mworld_render_utils camera shader blocks)
set_target_properties(block_renderer PROPERTIES LINKER_LANGUAGE CXX)