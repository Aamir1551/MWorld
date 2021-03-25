cmake_minimum_required(VERSION ${using_cmake_version})
project(util_functions)
set(CMAKE_CXX_STANDARD 11)

add_library(utils "${CMAKE_CURRENT_SOURCE_DIR}/util_functions.h" "${CMAKE_CURRENT_SOURCE_DIR}/util_functions.cpp")
target_include_directories(utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.")