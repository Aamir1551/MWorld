cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(lin_alg_path "${CMAKE_CURRENT_SOURCE_DIR}/../../numerics/linear_alg/include/")
message(${lin_alg_path})

project(cube)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../numerics/linear_alg" "./linear_alg")

add_library(cube "${CMAKE_CURRENT_SOURCE_DIR}/cube.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/cube.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/cube_collection.hpp")
#add_library(cube_collection "${CMAKE_CURRENT_SOURCE_DIR}/cube_collection.hpp")

target_include_directories(cube PUBLIC "${lin_alg_path}")
#target_include_directories(cube_collection PUBLIC "${lin_alg_path}")
#target_include_directories(cube_collection PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")
target_include_directories(cube PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")
target_link_libraries(cube linear_alg)
#target_link_libraries(cube_collection linear_alg)