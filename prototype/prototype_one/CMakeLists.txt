cmake_minimum_required(VERSION 3.10)
project(prototype_one)
set(CXX_STANDARD 11)

add_executable(prototype_one main.cpp cube.cpp)
target_link_libraries(prototype_one PUBLIC linear_alg quaternion cubes_collection_render)
target_link_libraries(prototype_one PUBLIC utils glad "${glfw}")