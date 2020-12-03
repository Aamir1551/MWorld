cmake_minimum_required(VERSION 3.10)

project(render)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/cubes_collection_render")

add_executable(tut cube_render.cpp)

target_include_directories(tut PUBLIC "${glfw_include_path}" "${glm_include_path}"  "${CMAKE_CURRENT_SOURCE_DIR}/.")
target_link_libraries(tut PUBLIC utils glad "${glfw}" cubes_collection_render)