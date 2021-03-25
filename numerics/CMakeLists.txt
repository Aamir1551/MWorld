cmake_minimum_required(VERSION ${using_cmake_version})
set(CMAKE_CXX_STANDARD 11)
project(numerics)

add_subdirectory(linear_alg)
add_subdirectory(quaternion)