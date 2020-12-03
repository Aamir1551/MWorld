cmake_minimum_required(VERSION 3.10)
project(mworld_render_utils)
set(CMAKE_CXX_STANDARD 11)

add_library(mworld_render_utils INTERFACE)
target_sources(mworld_render_utils INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/world_initializer.hpp")
target_include_directories(mworld_render_utils INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/.")