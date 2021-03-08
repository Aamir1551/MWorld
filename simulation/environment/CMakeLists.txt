cmake_minimum_required(VERSION 3.10)
project(simulation)
set(CXX_STANDARD 11)

add_executable(simulation simulation.cpp)
add_executable(or_gate_test or_gate_test.cpp)
add_executable(and_gate_test and_gate_test.cpp)
add_executable(not_gate_test not_gate_test.cpp)
add_executable(amplifier amplifier.cpp)

set(libraries_required "block_renderer" "linear_alg" "quaternion" "cube_renderer" "utils" "glad" ${glfw} "settings"  "mworld_render_utils" "camera" "shader" "blocks" "world_handler")
set(includes_required ${glfw_include_path} ${glm_include_path} ${CMAKE_CURRENT_SOURCE_DIR}/ )


if(compile_with_glfw)
    target_compile_definitions(simulation PRIVATE GLFW_ON=1)
endif()
target_compile_definitions(simulation PRIVATE uu=1)

target_link_libraries(simulation PUBLIC ${libraries_required})
target_link_libraries(or_gate_test PUBLIC ${libraries_required})
target_link_libraries(and_gate_test PUBLIC ${libraries_required})
target_link_libraries(not_gate_test PUBLIC ${libraries_required})
target_link_libraries(amplifier PUBLIC ${libraries_required})

target_include_directories(simulation PUBLIC ${includes_required})
target_include_directories(or_gate_test PUBLIC ${includes_required})
target_include_directories(and_gate_test PUBLIC ${includes_required})
target_include_directories(not_gate_test PUBLIC ${includes_required})
target_include_directories(amplifier PUBLIC ${includes_required})