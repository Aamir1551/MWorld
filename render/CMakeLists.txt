cmake_minimum_required(VERSION 3.10)

project(render)

set(CMAKE_CXX_STANDARD 11)
set(glfw "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.2/build/src/libglfw3.a")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/glad")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/cube")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../utils" "./util_build")

add_executable(tut cube_render.cpp)

target_include_directories(tut PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.2/include/" "${CMAKE_CURRENT_SOURCE_DIR}/cube"  "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm" )
target_include_directories(tut PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.")

target_link_libraries(tut PUBLIC cube)
target_link_libraries(tut PUBLIC utils glad "${glfw}" cube)