cmake_minimum_required(VERSION ${using_cmake_version})
project(linear_alg_test)

set(CMAKE_CXX_STANDARD 11)

add_executable(matrix_test test.cpp test-main.cpp)

target_include_directories(matrix_test PUBLIC ${catch_path})

target_link_libraries(matrix_test PUBLIC linear_alg)