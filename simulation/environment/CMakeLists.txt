cmake_minimum_required(VERSION ${using_cmake_version})
project(simulation)
set(CXX_STANDARD 11)

add_executable(simulation simulation.cpp)
add_executable(octree_test octree_test.cpp)
add_executable(barnes_hut_test barnes_hut_test.cpp)

if(compile_with_glfw)
    add_executable(or_gate_test or_gate_test.cpp)
    add_executable(and_gate_test and_gate_test.cpp)
    add_executable(not_gate_test not_gate_test.cpp)
    add_executable(amplifier amplifier.cpp)
endif()

set(libraries_required  "linear_alg" "quaternion" "utils" "settings" "blocks" "world_handler")
set(includes_required ${CMAKE_CURRENT_SOURCE_DIR}/ )

if(compile_with_glfw)

    #required libs are:  block_renderer linear_alg quaternion cube_renderer utils glad ${glfw} settings  mworld_render_utils camera shader blocks world_handler)
    set(libraries_required ${libraries_required} "block_renderer" "cube_renderer" "glad" ${glfw} "mworld_render_utils" "camera" "shader" )
    set(includes_required ${glfw_include_path} ${CMAKE_CURRENT_SOURCE_DIR}/ )

    target_compile_definitions(simulation PRIVATE GLFW_ON=1)
    target_compile_definitions(octree_test PRIVATE GLFW_ON=1)
    target_compile_definitions(barnes_hut_test PRIVATE GLFW_ON=1)

    target_link_libraries(or_gate_test PUBLIC ${libraries_required})
    target_link_libraries(and_gate_test PUBLIC ${libraries_required})
    target_link_libraries(not_gate_test PUBLIC ${libraries_required})
    target_link_libraries(amplifier PUBLIC ${libraries_required})

    target_include_directories(or_gate_test PUBLIC ${includes_required})
    target_include_directories(and_gate_test PUBLIC ${includes_required})
    target_include_directories(not_gate_test PUBLIC ${includes_required})
    target_include_directories(amplifier PUBLIC ${includes_required})
endif()

target_link_libraries(simulation PUBLIC ${libraries_required})
target_include_directories(simulation PUBLIC ${includes_required})

target_link_libraries(octree_test PUBLIC ${libraries_required})
target_include_directories(octree_test PUBLIC ${includes_required})

target_link_libraries(barnes_hut_test PUBLIC ${libraries_required})
target_include_directories(barnes_hut_test PUBLIC ${includes_required})
