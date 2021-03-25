cmake_minimum_required(VERSION ${using_cmake_version})
project(shader)
set(CMAKE_CXX_STANDARD 11)

add_library(shader INTERFACE)
target_sources(shader INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/shader.hpp")
target_include_directories(shader INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/")
target_link_libraries(shader INTERFACE utils)