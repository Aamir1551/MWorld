cmake_minimum_required(VERSION 3.10)
project(prototype_one)
set(CXX_STANDARD 11)
set(glfw "${CMAKE_CURRENT_SOURCE_DIR}/../../render/lib/glfw-3.3.2/build/src/libglfw3.a")

add_executable(prototype_one cube.cpp main.cpp)
target_link_libraries(prototype_one PUBLIC linear_alg quaternion cubes_collection_render)
target_link_libraries(prototype_one PUBLIC utils glad "${glfw}" settings)

target_include_directories(prototype_one PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../render/lib/glfw-3.3.2/include/"  "${CMAKE_CURRENT_SOURCE_DIR}/../../lib/glm" )
target_include_directories(prototype_one PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../render/")