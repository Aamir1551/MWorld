cmake_minimum_required(VERSION ${using_cmake_version})
project(cube_renderer CXX)
set(CMAKE_CXX_STANDARD 11)
set(project_name cube_renderer)


add_library(cube_renderer "${CMAKE_CURRENT_SOURCE_DIR}/cube_renderer.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/cube_renderer.cpp" )

target_include_directories(cube_renderer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/." "${CMAKE_CURRENT_SOURCE_DIR}/../lib/" "${glfw_include_path}" "${glm_include_path}" )
target_link_libraries(cube_renderer PUBLIC glad "${glfw}" settings)
set_target_properties(cube_renderer PROPERTIES LINKER_LANGUAGE CXX)