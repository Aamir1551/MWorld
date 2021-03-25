cmake_minimum_required(VERSION ${using_cmake_version})
project(quaternion_lib)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(test)

add_library(quaternion "${CMAKE_CURRENT_SOURCE_DIR}/include/quaternion.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/quaternion.cpp")
target_include_directories(quaternion PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(quaternion PUBLIC linear_alg settings)