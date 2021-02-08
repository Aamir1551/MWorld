cmake_minimum_required(VERSION 3.10)
project(quaternion_test)

set(CMAKE_CXX_STANDARD 11)

add_executable(quaternion_test test.cpp test-main.cpp)

target_include_directories(quaternion_test PUBLIC ${catch_path})
target_link_libraries(quaternion_test PUBLIC quaternion)