cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
project(utils)

add_library(settings "${CMAKE_CURRENT_SOURCE_DIR}/settings.h")
set_target_properties(settings PROPERTIES LINKER_LANGUAGE CXX)

add_library(utils "${CMAKE_CURRENT_SOURCE_DIR}/utils.h" "${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp")
target_include_directories(utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.")