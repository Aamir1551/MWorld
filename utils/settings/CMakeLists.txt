cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
project(settings)

add_library(settings INTERFACE)
target_sources(settings INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/settings.hpp")
target_include_directories(settings INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/.")