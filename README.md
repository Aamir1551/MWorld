# MWorld  
A simulation of M world, as described in the report in the MWorld section.

Building the Project:
1) To build the project copy the folder MWorld into local home directory.
2) Use a build system to build using the CMakefiles.
3) To render the virtual world ensure that you have GLFW and GLAD installed on your system, with file paths linked correctly with the program. File paths for GLFW and GLAD should be provided on lines 55-60 on the root CMakelists.txt file.
4) To compile with OpenMP, set line 15 in the root CMakeLists.txt file to: set(compile_with_openmp ON). To compile without set line 15 to: set(compile_with_openmp OFF)
5) To compile with GLFW, set line 16 in the root CMakeLists.txt file to: set(compile_with_glfw OFF). To compile without set line 16 to: set(compile_with_glfw OFF)
6) To compile with prototypes, set line 17 in the root CMakeLists.txt file to: set(compile_prototypes ON). To compile without set line 17 to: set(compile_prototypes OFF)

Running the Project:
1) After building the project, execute the executable made by the file simulation.cpp in the directory simulation/environment. 
2) The number of blocks used within the world can be changed in this file via the method WorldHandler. In the argument for this method, pass in the amount of different blocks that you would like to simulate in the order: 
