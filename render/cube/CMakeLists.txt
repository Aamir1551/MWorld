cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

project(cube)

add_library(cube INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/cube.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/cube_collection.hpp")

target_include_directories(cube INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/")