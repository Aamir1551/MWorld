#include <cstring>
#include <unordered_set>
#include <set>

#if defined(GLFW_ON)
    #include <glad/glad.h>
    #include <GLFW/glfw3.h>
#endif

#if defined(GLFW_ON)
    #include <world_initializer.hpp>
    #include <camera.hpp>
    #include <block_renderer.hpp>
#endif

#include <world_handler.hpp>
#include <settings.hpp>
#include <matrix.hpp>
#include <omp.h>
#include <chrono>

using namespace std;


#if defined(GLFW_ON)
    using namespace render_utils;
#endif

using namespace numerics;
using namespace settings;
using namespace blocks;

int main(int argc, char *argv[])
{
    if(argc == 1 || strcmp(argv[1], "help")  == 0){
        cout << "Pass parameters in order of: duration num_i_plus_blocks num_i_neg_blocks num_z_blocks num_m_blocks "
                "num_e_1_blocks num_e_1_2_blocks min_coord_x max_coord_x min_coord_y max_coord_y min_coord_z max_coord_z"<< endl;
        cout << "If a parameter is not given, we substitute it with zero. However, duration command line argument "
                "must be specified." << endl;
        exit(0);
    }



#if defined(GLFW_ON)
    // Setting up GLFW
    WorldProperties *world_properties = WorldIntializer();
    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
#endif


#if defined(GLFW_ON)
    // Setting camera functions
    real cube_length = 4.0f;
    Camera camera = Camera(world_properties->window);
    camera.camera_pos_mat = Matrix::CreateColumnVec(0, 0, 250);
    BlockRenderer::InitialiseBlockRenderer(&camera, cube_length, vao, vbo, ebo, world_properties);
#endif

    WorldHandler world = WorldHandler((argc < 3) ? 100 : (int) atoi(argv[2]),
                                      (argc < 4) ? 100 : (int) atoi(argv[3]),
                                      (argc < 5) ? 100 : (int) atoi(argv[4]),
                                      (argc < 6) ? 100 : (int) atoi(argv[5]),
                                      (argc < 7) ?100 : (int) atoi(argv[6]),
                                      (argc < 8) ? 100 : (int) atoi(argv[7]),
                                      (argc < 9) ? -100 : (int) atoi(argv[8]),
                                      (argc < 10) ? 100 : (int) atoi(argv[9]),
                                      (argc < 11) ? -100 : (int) atoi(argv[10]),
                                      (argc < 12) ? 100 : (int) atoi(argv[11]),
                                      (argc < 13) ? -100 : (int) atoi(argv[12]),
                                      (argc < 14) ? 100 : (int) atoi(argv[13]),
                                      (argc < 15) ? 1 : (int) atoi(argv[14]),4);

#if defined(GLFW_ON)
    glBindVertexArray(vao);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUseProgram(world_properties->shader_id);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //using black to clear the background
#endif

    // Setting time functions
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    auto start_time = high_resolution_clock::now();
    auto currentFrame = high_resolution_clock::now();
    auto lastFrame = high_resolution_clock::now(); // Time of last frame
    auto deltaTime = duration_cast<milliseconds>(currentFrame - lastFrame); // time between current frame and last frame

    real frame_count = 0;
    real total_frame_count = 0;

    auto prev_time =  high_resolution_clock::now();


    while (
#if defined(GLFW_ON)
         !glfwWindowShouldClose(world_properties->window) && duration_cast<milliseconds>(currentFrame - start_time).count() < atoi(argv[1]) * 1000
#else
            duration_cast<milliseconds>(currentFrame - start_time).count() < atoi(argv[1]) * 1000 // time between current frame and last frame is less than 1st argument given
#endif
    )
    {
       currentFrame = high_resolution_clock::now();
       deltaTime = duration_cast<milliseconds>(currentFrame - lastFrame); // Time between current frame and last frame
       lastFrame = currentFrame;

#if defined(GLFW_ON)
        camera.UpdateCamera(deltaTime.count() /1000.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif


        // Get all world contacts using the octree. Comment below line, if you would like to use Brute force approach instead.
        auto contact_list = world.CollisionHandlerWithOctree();

        // Get all world contacts using the brute force algorithm. Uncomment this line, to use brute force algorithm instead.
        // auto contact_list = world.CollisionHandlerBruteForce();

        // Apply forces to blocks using the BH algorithm. Comment this line, if you would like to use Brute force approach instead.
        world.AddForcesViaBarnesHut(deltaTime.count() / 1000.0);

        // Uncomment out below line of code, to use the brute force approach to calculate the forces between blocks.
        //world.AddForcesViaBruteForce(deltaTime.count() / 1000.0);

        // Update block positions
        world.Update(contact_list, deltaTime.count()/1000.0);

#if defined(GLFW_ON)
        BlockRenderer::DrawAllBlocks(&world.iblocks, &world.zblocks, &world.eblocks, &world.mblocks);
        glfwSwapBuffers(world_properties->window);
        glfwPollEvents();
#endif
        frame_count++;
        total_frame_count++;
        if(duration_cast<milliseconds>(currentFrame - prev_time).count()>= 1000.0) {
            cout << "FPS: " << frame_count << endl;
            frame_count = 0;
            prev_time = currentFrame;
        }
    }

    cout << "Average execution Time per frame when simulating with block tuple ("
    <<  ((argc < 3) ? "100" : argv[2]) << ", " << ((argc < 4) ? "100" : argv[3]) << ", " << ((argc < 5) ? "100" : argv[4]) << ", " << ((argc < 6) ? "100" : argv[5]) << ", "
    <<  ((argc < 7) ? "100" : argv[6]) << ", " << ((argc < 8) ? "100" : argv[7]) << "), and world dimension tuple: (x_min: "
    <<  ((argc < 9) ? "-100" : argv[8]) << ", x_max: " << ((argc < 10) ? "100" : argv[9]) << ", y_min: "
    <<  ((argc < 11) ? "-100" : argv[10]) << ", y_max: " << ((argc < 12) ? "100" : argv[11]) << ", z_min: "
    <<  ((argc < 13) ? "-100" : argv[12]) << ", z_max: " << ((argc < 14) ? "100" : argv[13]) << ") is: " <<
    (duration_cast<milliseconds>(high_resolution_clock::now() - start_time).count()) / total_frame_count / 1000 << endl;

#if defined(GLFW_ON)
    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glfwTerminate();
#endif

    return 0;
}