cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

project(linear_alg)

add_library(linear_alg
    include/matrix.h
    src/matrix.cpp
)




target_include_directories(
  linear_alg
  PUBLIC
  "include"
)

add_executable(test
    test/test.cpp
)

target_include_directories(
  test
  PUBLIC
  "../../catch"
)

target_link_libraries(test linear_alg)