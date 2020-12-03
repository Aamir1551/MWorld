cmake_minimum_required(VERSION 3.10)
project(prototype_one)
set(CXX_STANDARD 11)

add_executable(prototype_one cube.cpp main.cpp)
target_link_libraries(prototype_one PUBLIC linear_alg quaternion cubes_collection_render)
target_link_libraries(prototype_one PUBLIC utils glad "${glfw}" settings)

target_include_directories(prototype_one PUBLIC "${glfw_include_path}"  "${glm_include_path}" )
target_include_directories(prototype_one PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../render/")