cmake_minimum_required(VERSION 3.10)
project(linear_alg_test)

set(CMAKE_CXX_STANDARD 11)

add_executable(test test.cpp test-main.cpp)

target_include_directories(test PUBLIC "../../../catch")

target_link_libraries(test PUBLIC linear_alg)