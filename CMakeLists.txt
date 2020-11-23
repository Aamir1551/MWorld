cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

project(MWorld)
add_subdirectory(render) #[[make sure to configure vscode so that it uses ninja instead of makefiles]]
add_subdirectory(numerics)