cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

project(cube CXX)

#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../lib/glad")

add_library(cube "${CMAKE_CURRENT_SOURCE_DIR}/cube.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/cube.cpp" )

target_include_directories(cube PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/../lib/" )
target_include_directories(cube PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../lib/glfw-3.3.2/include/" "${CMAKE_CURRENT_SOURCE_DIR}/../lib/glm" )
target_include_directories(cube PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(cube glad glfw)

set_target_properties(cube PROPERTIES LINKER_LANGUAGE CXX)