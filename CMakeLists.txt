set(using_cmake_version "3.7")
cmake_minimum_required(VERSION ${using_cmake_version})

#export CC=C:\Program Files\LLVM\bin\clang.exe
#export CXX=C:\Program Files\LLVM\bin\clang++.exe

set(CMAKE_CXX_STANDARD 11)
#set(glfw "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.2/build/src/libglfw3.a")
#set(glfw_include_path "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.2/include/")
set(glm_include_path "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm") #Used for testing the matrix LookAt and Perspective function
set(catch_path ${CMAKE_CURRENT_SOURCE_DIR}/catch)

#multithreading depends heavily on what applications are running in the background and such

set(compile_with_openmp ON)
set(compile_with_glfw OFF)
set(compile_prototypes ON)
set(add_debug_tag OFF)

#add_compile_options("-mavx" "-mavx2" "-O3" "-ftree-vectorize") #-- make sure this adds these flags to all targets, rendering stops for some reason, if we use -O3 and -O2
if(compile_with_openmp)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fopenmp -std=c++11 -qopenmp")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
endif()

if(add_debug_tag)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    set(CMAKE_BUILD_TYPE Debug)
endif()


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

if(NOT compile_with_glfw)
    set(compile_prototypes OFF)
endif()


project(MWorld)

add_subdirectory(numerics)
link_libraries(linear_alg quaternion)

if(compile_with_glfw)
    set(glfw "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.2/build/src/libglfw3.a")
    set(glfw_include_path "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.2/include/")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib/glad")
    add_subdirectory(render)
endif()

add_subdirectory(utils)
if(compile_prototypes)
    add_subdirectory(prototype)
endif()

#add_subdirectory(catch)
add_subdirectory(simulation)
