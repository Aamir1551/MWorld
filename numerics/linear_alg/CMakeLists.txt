cmake_minimum_required(VERSION 3.10)
project(linear_alg)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(test)

add_library(linear_alg include/matrix.hpp include/matrix3.hpp src/matrix.cpp src/matrix3.cpp)

target_link_libraries(linear_alg PUBLIC settings)
target_include_directories(linear_alg PUBLIC "include")