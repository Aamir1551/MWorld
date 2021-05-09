# MWorld  
A simulation of M world, as described in the report in the MWorld section.

Versions 3.3 of GLFW is required for this project. Ensure that you have version 9 of GCC installed on your machine - so that you may use OpenMP (since version OpenMP 5.0 has been used for this project). Depending upon machine being used, you may need to use additional libraries and flags to ensure GLFW works correctly. This can be done via inserting the line of code ```set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Flag -Lib")}``` in the root CMakeLists.txt file. Finally, minimum CMake version required for this project is 3.10 

Video evidence of the project compiling and being ran is provided here: 

Note that the video evidence was produced on the machine specified in Appendix E.

When referencing the root CMakefile, we are specifically talking about the file $\texttt{CMakefile.txt}$ in the MWorld folder.

More indepth detail regarding how to build this project is specified on Appendix D of the report.
Building the Project via command line:
1) Copy the folder MWorld into local home directory, via a git clone.
2) In the folder MWorld, create a folder called Build. We will build the project in this directory.
3) Change directory into the build folder.
4) Run the command ```cmake .. -G Ninja```, to build using Ninja as the generator. Any other alternatives would do; however, for this project we used Ninja to build our project.
5) Finally, run the command ```ninja```, in the build directory, to build the project. If another generator was used by cmake instead of ninja, use that instead to build the project.
6) To render the virtual world ensure that you have GLFW and Glad installed on your system, with file paths linked correctly with the program. File paths for GLFW and Glad should be provided on lines 55-58 on the root CMakelists.txt file.
7) To compile with OpenMP, set line 15 in the root CMakeLists.txt file to: ```set(compile_with_openmp ON)```. To compile without set line 15 to: ```set(compile_with_openmp OFF)```
8) To compile with GLFW, set line 16 in the root CMakeLists.txt file to: ```set(compile_with_glfw ON)```. To compile without set line 16 to: ```set(compile_with_glfw OFF)```
9) To compile with prototypes, set line 17 in the root CMakeLists.txt file to: ```set(compile_prototypes ON)```. To compile without set line 17 to: ```set(compile_prototypes OFF)```. In addition, ensure that GLFW has also been enabled, since prototypes will only be compiled if line 16 is set to ```set(compile_with_glfw ON)```.

Running the Project:
1) In the build folder, where we built the project, change directory to ./simulation/environment.
2) Run the executable: ```./simulation```. Command line arguments can be passed in the following order: ```(duration to run simulation for [Required], No. of I+ blocks to simulate [Optional], No. of I- blocks to simulate [Optional], No. of Z blocks to simulate [Optional], No. of M blocks to simulate [Optional], No. of E+ blocks to simulate [Optional], No. of E- blocks to simulate [Optional], world min_x [Optional], world max_x [Optional], world min_y [Optional], world max_y [Optional], world min_z [Optional], world max_z [Optional], No. threads to use [Optional])```. If arguments are not provided for the blocks, by default we use 100 of that type. The values world min and world max refer to the minimum and maximum coordinates our blocks can have in a given axes. If value of ```world min``` is not provided, it is set to −100, and if value of ```world max``` is not provided it is set to 100 by default. Finally, the value ```No. threads to use``` refers to the number of threads to execute the project. By default, this value is set to 1. This value is only utilised when we compile with OpenMP, i.e line 15 in the root CMakeLists.txt has been set to ```set(compile_with_openmp ON)```.
3) To utilise the brute force algorithms instead of the octree for collision detection and the BH algorithm for force calculations, make the appropriate changes to the simulation.cpp file, as explained in the comments [line 119-129].
4) To run the simulations of the logic gates, execute files:
    1) ```and_gate_test``` to run the simulation with the AND gate being simulated
    2) ```not_gate_test``` to run the simulation with the NOT gate being simulated
    3) ```or_gate_test``` to run the simulation with the OR gate being simulated. Noote that GLFW must be enabled and linked to the build system in order to build these files.
5) Unit test files for the Matrix and Quaternion class should be located in: ```numerics/linear_alg/test/test.cpp```, ```numerics/quaternion/test/test.cpp```. These files can be executed without the need to pass in command line arguments, and their corresponding executables after being compiled should be located in ```build/numerics/linear_alg/test/matrix_test``` and ```build/numerics/quaternion/test/quaternion_test```
6) Finally, tests for octree and BH algorithm are located in the folder ```simulation/tree_integration_tests```. For these tests, passing in command line arguments is similar to that of passing in arguments to the ```./simulation``` executable, however in this case all parameters are required. In addition, for these tests, we do not specify the number of threads used (that can be changed in the .cpp file itself). Finally, the file ```barnes_hut_test``` requires an additional final parameter specifying the value of θ to be used as a hyper-parameter. Ensure that when running these tests, GLFW is turned off, i.e line 16 of root CMakeLists.txt file should be set to ```set(compile_with_glfw OFF)```.

Code Structure:
1) The Matrix and Quaternion code is written in the ```numerics``` folder.
2) The three prototypes used can be found in the ```prototype``` folder
3) The ```render``` folder contains code specific to rendering using GLFW.
4) The ```simulation``` folder includes five more directories:
    1) ```blocks``` - Contains code to simulate each block within the world.
    2) ```octree``` - Contains code related to the ForceOctree (Used by the Barnes-Hut algorithm) and CollisionOctree used by the collision handler.
    3) ```world_handler``` - Calls the methods that run at each frame in the physics engine. It is here, where we use make use of the Block and Octree classes in our simulation.
    4) ```environment``` - Contains the different environments we have simulated such as the logic gates and the final simulation of the world.
    5) ```tree_integration_tests``` - Contains test code to test the octree collision detection and BH algorithm.
5) ```Util``` contains auxiliary functions that we make use of.
6) ```lib``` contains the libraries used throughout the project, which includes GLFW/GLAD and glm, the latter which is used to test the matrix file, to see if its rendering functions are working correctly.
7) ```results``` contain the sub directories:
    1) ```execution_results``` - Contains the timed execution results, generated via automated tests. Results are presented in chapter 6
    2) ```integration_test_results``` - Contains the results of testing the octree for collision detection and the BH algorithm, for correctness.
    3) ```memory_test_results``` - Contains the output of Valgrind when running the simulation with different hyperparameters. Results are represented in chapter 6. In this folder file: ```massif.out.L_N``` represents the massif output when using a world dimension of L × L × L and N number of blocks of each type.
8) ```test_scripts``` - Folder contains the python scripts used to automate the testing procedure for the octrees
9) Finally, we have the folder ```catch``` which includes the catch test library used to help us unit test our project.
