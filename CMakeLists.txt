cmake_minimum_required(VERSION 3.10)

#export CC=C:\Program Files\LLVM\bin\clang.exe
#export CXX=C:\Program Files\LLVM\bin\clang++.exe

set(CMAKE_CXX_STANDARD 11)
set(glfw "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.2/build/src/libglfw3.a")
set(glfw_include_path "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.2/include/")
set(glm_include_path "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm")
set(catch_path ${CMAKE_CURRENT_SOURCE_DIR}/catch)


#SET (CMAKE_C_FLAGS_INIT                "-Wall -std=c99")
#SET (CMAKE_C_FLAGS_DEBUG_INIT          "-g")
#SET (CMAKE_C_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
#SET (CMAKE_C_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
#SET (CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

#SET (CMAKE_CXX_FLAGS_INIT                "-Wall")
#SET (CMAKE_CXX_FLAGS_DEBUG_INIT          "-g")
#SET (CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
#SET (CMAKE_CXX_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
#SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

project(MWorld)
#add_subdirectory(catch)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/glad")
add_subdirectory(utils)
add_subdirectory(numerics)
add_subdirectory(render)
add_subdirectory(prototype)
add_subdirectory(simulation)
