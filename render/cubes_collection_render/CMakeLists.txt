cmake_minimum_required(VERSION 3.10)
project(cubes_collection_render CXX)
set(CMAKE_CXX_STANDARD 11)
set(glfw "${CMAKE_CURRENT_SOURCE_DIR}/../lib/glfw-3.3.2/build/src/libglfw3.a")


add_library(cubes_collection_render "${CMAKE_CURRENT_SOURCE_DIR}/cubes_collection_render.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/cubes_collection_render.cpp" )

target_include_directories(cubes_collection_render PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/../lib/" )
target_include_directories(cubes_collection_render PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../lib/glfw-3.3.2/include/" "${CMAKE_CURRENT_SOURCE_DIR}/../lib/glm" )
target_include_directories(cubes_collection_render PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.")

target_link_libraries(cubes_collection_render glad "${glfw}")

set_target_properties(cubes_collection_render PROPERTIES LINKER_LANGUAGE CXX)