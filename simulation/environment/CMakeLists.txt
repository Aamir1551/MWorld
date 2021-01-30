cmake_minimum_required(VERSION 3.10)
project(simulation)
set(CXX_STANDARD 11)

add_executable(simulation simulation.cpp)
add_executable(logic_gates logic_gates.cpp)
add_executable(flare_test flare_test.cpp)
target_link_libraries(simulation PUBLIC block_renderer linear_alg quaternion cube_renderer utils glad ${glfw} settings  mworld_render_utils camera shader blocks)
target_link_libraries(logic_gates PUBLIC block_renderer linear_alg quaternion cube_renderer utils glad ${glfw} settings  mworld_render_utils camera shader blocks)
target_link_libraries(flare_test PUBLIC block_renderer linear_alg quaternion cube_renderer utils glad ${glfw} settings  mworld_render_utils camera shader blocks)

target_include_directories(simulation PUBLIC "${glfw_include_path}"  "${glm_include_path}" "${CMAKE_CURRENT_SOURCE_DIR}/" )
target_include_directories(logic_gates PUBLIC "${glfw_include_path}"  "${glm_include_path}" "${CMAKE_CURRENT_SOURCE_DIR}/" )
target_include_directories(flare_test PUBLIC "${glfw_include_path}"  "${glm_include_path}" "${CMAKE_CURRENT_SOURCE_DIR}/" )
