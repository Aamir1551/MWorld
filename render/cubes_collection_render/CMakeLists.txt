cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(glfw "${CMAKE_CURRENT_SOURCE_DIR}/../lib/glfw-3.3.2/build/src/libglfw3.a")
set(file_name "cubes_collection_render")

project(${file_name} CXX)

add_library(${file_name} "${CMAKE_CURRENT_SOURCE_DIR}/${file_name}.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/${file_name}.cpp" )

target_include_directories(${file_name} PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/../lib/" )
target_include_directories(${file_name} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../lib/glfw-3.3.2/include/" "${CMAKE_CURRENT_SOURCE_DIR}/../lib/glm" )
target_include_directories(${file_name} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(${file_name} glad "${glfw}")

set_target_properties(${file_name} PROPERTIES LINKER_LANGUAGE CXX)