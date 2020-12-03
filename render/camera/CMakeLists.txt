cmake_minimum_required(VERSION 3.10)
project(camera)
set(CMAKE_CXX_STANDARD 11)

add_library(camera INTERFACE)
target_sources(camera INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/camera.hpp")
target_include_directories(camera INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/")