# MWorld  
A simulation of M world, as described in the report in the MWorld section.

Building the Project:
1) To build the project copy the folder MWorld into local home directory.
2) Use a build system to build using the CMakefiles. We used Ninja; however, any build system that can process CMakeLists.txt files would do.
3) To render the virtual world ensure that you have GLFW and Glad installed on your system, with file paths linked correctly with the program. File paths for GLFW and Glad should be provided on lines 55-60 on the root CMakelists.txt file.
4) To compile with OpenMP, set line 15 in the root CMakeLists.txt file to: set(compile_with_openmp ON). To compile without set line 15 to: set(compile_with_openmp OFF)
5) To compile with GLFW, set line 16 in the root CMakeLists.txt file to: set(compile_with_glfw OFF). To compile without set line 16 to: set(compile_with_glfw OFF)
6) To compile with prototypes, set line 17 in the root CMakeLists.txt file to: set(compile_prototypes ON). To compile without set line 17 to: set(compile_prototypes OFF)

Note that the software can be executed without the use of GLFW and GLAD; however, you will not be able render the simulation, but the world will still be simulated but just not rendered to the screen. To build the project without the use of GLFW, ensure to set line 16 of the file CMakelists.txt to: set(compile_with_glfw OFF)

Running the Project:
1) The number of blocks used within the world and world size can be set in the simulation.cpp file via the constructor WorldHandler. The argument signature for this constructor is given by: (num I+ blocks, num I- blocks, num Z blocks, num MBlocks, num E+ blocks, num E- blocks, minimum x coordinate a block can take in the world = -50 by default, max x coordinate = 50 by default, min y coordinate = -50 by default, max y coordinate = 50 by default, min z coordinate = -50 by default, max z coordinate = 50 by default, length of cubes = 4 by default)
2) Build the project as explained above
3) Execute the executable simulation made by compiling the file ./simulation.cpp the directory simulation/environment. If the project was compiled without GLFW then provide a command line argument specifying the duration to run the simulation for. If the project was compiled with GLFW, then to escape the simulation, you may press ESC. 
4) To run the simulations of the logic gates, execute files:
    1) and_gate_test to run the simulation with the AND gate being simulated
    2) not_gate_test to run the simulation with the NOT gate being simulated
    3) or_gate_test to run the simulation with the OR gate being simulated

Code Structure:
1) The Matrix and Quaternion code is writen in the numerics folder. 
2) The three prototypes used can be found in the prototypes folder. 
3) The render folder contains code specific to rendering using GLFW. 
4) The simulation folder includes two more directories: 
    1) blocks - Contains code related to each the functionality to the blocks within the world
    2) octree - Contains code related to the ForceOctree (Used by the Barnes-Hut algorithm) and CollisionOctree used by the collision handler
    3) world_handler - Calls the methods that run at each frame in the physics engine. It is here, where we use make use of the Block and Octree classes in our simulation.
    4) environment - Contains the different environments we have simulated such as the logic gates and the final simulation of the world
5) Util contains auxiliary functions that we make use of. 
6) Lib contains the libraries used throughout the project, which include GLFW/GLAD and glm, the latter which is used to test the matrix file, to see if its rendering functions are working correctly.
7) Finally, we have the folder catch which includes the catch test library used to help us unit test our project.
