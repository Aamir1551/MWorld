cmake_minimum_required(VERSION ${using_cmake_version})
project(linear_alg)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(test)

add_library(linear_alg include/matrix.hpp src/matrix.cpp)

target_link_libraries(linear_alg PUBLIC settings)
target_compile_options(linear_alg PUBLIC "-mavx" "-mavx2")
target_include_directories(linear_alg PUBLIC "include")