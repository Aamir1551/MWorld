# MWorld  
A simulation of M world, as described in the report in the MWorld section.

Building the Project:
1) To build the project copy the folder MWorld into local home directory.
2) Use a build system to build using the CMakefiles.
3) To render the virtual world ensure that you have GLFW and Glad installed on your system, with file paths linked correctly with the program. File paths for GLFW and Glad should be provided on lines 55-60 on the root CMakelists.txt file.
4) To compile with OpenMP, set line 15 in the root CMakeLists.txt file to: set(compile_with_openmp ON). To compile without set line 15 to: set(compile_with_openmp OFF)
5) To compile with GLFW, set line 16 in the root CMakeLists.txt file to: set(compile_with_glfw OFF). To compile without set line 16 to: set(compile_with_glfw OFF)
6) To compile with prototypes, set line 17 in the root CMakeLists.txt file to: set(compile_prototypes ON). To compile without set line 17 to: set(compile_prototypes OFF)

Note that the software can be executed without the use of GLFW and GLAD; however, you will not be able render the simulation, but the world will still be simulated but just not rendered to the screen. To build the project wihtout the use of GLFW, ensure to set line 16 of the file CMakelists.txt to: set(compile_with_glfw OFF)

Running the Project:
1) The number of blocks used within the world and world size can be set in this file via the method WorldHandler. The argument signiture for this method is given by: (num I+ blocks, num I- blocks, num Z blocks, num MBlocks, num E+ blocks, num E- blocks, minimum x coordinate a block can take in the world = -50 by default, max x coordinate = 50 by default, min y coordinate = -50 by default, max y coordinate = 50 by default, min z coordinate = -50 by default, max z coordinate = 50 by default, length of cubes = 4 by default)
2) Build the project as explained above
3) Execute the executable made by the file simulation.cpp in the directory simulation/environment. If the project was compiled wihtout GLFW then provide a command line argument specifying the duration to run the simulation for. If the project was compiled with GLFW, then to escape the simulation, you may press ESC. 
4) To run the simulations of the logic gates, execute files:
  1) and_gate_test to run the simulation with the AND gate being simulated
  2) not_gate_test to run the simulation with the NOT gate being simulated
  3) or_gate_test to run the simulation with the OR gate being simulated
