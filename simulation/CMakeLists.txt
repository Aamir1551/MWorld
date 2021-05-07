cmake_minimum_required(VERSION ${using_cmake_version})
project(simulation)
set(CXX_STANDARD 11)

add_subdirectory(blocks)
add_subdirectory(octree)
add_subdirectory(world_handler)
add_subdirectory(environment)
add_subdirectory(tree_integration_tests)
