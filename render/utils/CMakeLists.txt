cmake_minimum_required(VERSION ${using_cmake_version})
project(mworld_render_utils)
set(CMAKE_CXX_STANDARD 11)

add_library(mworld_render_utils "${CMAKE_CURRENT_SOURCE_DIR}/world_initializer.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/world_initializer.cpp")
target_sources(mworld_render_utils PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/world_initializer.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/world_initializer.cpp")
target_link_libraries(mworld_render_utils glad ${glfw} shader)
target_include_directories(mworld_render_utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/." "${glfw_include_path}" "${glm_include_path}")
