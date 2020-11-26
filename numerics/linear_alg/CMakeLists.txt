cmake_minimum_required(VERSION 3.10)
project(linear_alg)
set(CMAKE_CXX_STANDARD 11)


add_library(linear_alg include/matrix.h src/matrix.cpp)

target_include_directories(linear_alg PUBLIC "include")