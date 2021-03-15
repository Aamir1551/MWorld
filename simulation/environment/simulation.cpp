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
    //cout << "Running MWorld Simulation with "  << omp_get_max_threads() << " threads" << endl;

    if(argc == 1 || strcmp(argv[1], "help")  == 0){
        cout << "Pass parameters in order of: duration num_i_plus_blocks num_i_neg_blocks num_z_blocks num_m_blocks "
                "num_e_1_blocks num_e_1_2_blocks min_coord max_coord"<< endl;
        cout << "If a parameter is not given, we substitute it with zero. However, duration command line argument "
                "must be specified." << endl;
        exit(0);
    }

    cout << "Running MWorld Simulation" << endl;


#if defined(GLFW_ON)
    WorldProperties *world_properties = WorldIntializer();
    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
#endif


#if defined(GLFW_ON)
    real cube_length = 4.0f;
    Camera camera = Camera(world_properties->window);
    camera.camera_pos_mat = Matrix::CreateColumnVec(0, 0, 250);
    BlockRenderer::InitialiseBlockRenderer(&camera, cube_length, vao, vbo, ebo, world_properties);
#endif

    //int num_blocks_same = 100;
    //WorldHandler world = WorldHandler(num_blocks_same, num_blocks_same, num_blocks_same, num_blocks_same, num_blocks_same, num_blocks_same); //WorldHandler world = WorldHandler(0, 0, 0, 0, 0, 0); //WorldHandler world = WorldHandler(10, 0, 0, 0, 0, 0);
    //WorldHandler world = WorldHandler(0, 0, 2, 0, 0, 0);
    // Test out with different collision elasticity value, so change scaling of velocity after collision, and see how world develops
    /*WorldHandler world = WorldHandler((argc < 3) ? 100 : (int) atoi(argv[2]),
                                      (argc < 4) ? 100 : (int) atoi(argv[3]),
                                      (argc < 5) ? 100 : (int) atoi(argv[4]),
                                      (argc < 6) ? 100 : (int) atoi(argv[5]),
                                      (argc < 7) ?100 : (int) atoi(argv[6]),
                                      (argc < 8) ? 100 : (int) atoi(argv[7]),
                                      (argc < 9) ? -100 : (int) atoi(argv[8]),
                                      (argc < 10) ? 100 : (int) atoi(argv[9]),
    4);*/

    WorldHandler world = WorldHandler((argc < 3) ? 0 : (int) atoi(argv[2]),
                                      (argc < 4) ? 0 : (int) atoi(argv[3]),
                                      (argc < 5) ? 1000 : (int) atoi(argv[4]),
                                      (argc < 6) ? 0 : (int) atoi(argv[5]),
                                      (argc < 7) ? 0 : (int) atoi(argv[6]),
                                      (argc < 8) ? 0 : (int) atoi(argv[7]),
                                      (argc < 9) ? -100 : (int) atoi(argv[8]),
                                      (argc < 10) ? 100 : (int) atoi(argv[9]),
    4);

    // Testing with ZBLocks only -- Note we disabled the force's being applied, however, the forces are still being calculated
    // We shall have tests, with forces being applied and both not being applied
    // When doing time tests, also disable all glfw functions


    //100 = 170fps
    //200 = 75fps
    //300 = 38fps
    //400 = 25fps
    //500 = 20fps
    //600 = 15fps
    //700 = 12fps
    //800 = 9fps
    //900 = 8fps
    //1000 = 6fps

#if defined(GLFW_ON)
    glBindVertexArray(vao);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUseProgram(world_properties->shader_id);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //using black to clear the background
#endif
    std::cout << "Entering Main Loop" << std::endl;

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

#if defined(GLFW_ON)
    real currentFrame;
    real lastFrame; // Time of last frame
    real deltaTime; // Time between current frame and last frame
#else
    auto start_time = high_resolution_clock::now();
    auto currentFrame = high_resolution_clock::now();
    auto lastFrame = high_resolution_clock::now(); // Time of last frame
    auto deltaTime = duration_cast<milliseconds>(currentFrame - lastFrame); // time between current frame and last frame
#endif

    real frame_count = 0;

#if defined(GLFW_ON)
    real prev_time = glfwGetTime(); // prev_time refers to the last time we printed the num of frames per seconds
#else
    auto prev_time =  high_resolution_clock::now();
#endif


    while (
#if defined(GLFW_ON)
            !glfwWindowShouldClose(world_properties->window)
#else
            duration_cast<milliseconds>(currentFrame - start_time).count() < atoi(argv[1]) * 1000 // time between current frame and last frame
#endif
    )
    {
             #if defined(GLFW_ON)
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
#else
       currentFrame = high_resolution_clock::now();
       deltaTime = duration_cast<milliseconds>(currentFrame - lastFrame); // Time between current frame and last frame
       lastFrame = currentFrame;
#endif

#if defined(GLFW_ON)
        camera.UpdateCamera(deltaTime);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif

        //auto contact_list = world.CollisionHandlerBruteForce();
        //auto contact_list1 = world.CollisionHandlerWithOctree();

        auto contact_list = world.CollisionHandlerWithOctree();

        /*set<pair<Block *, Block *>> c;
        for(auto const &i : contact_list) {
           c.insert(make_pair(i.body1, i.body2));
           c.insert(make_pair(i.body2, i.body1));
        }

        set<pair<Block *, Block *>> c1;
        for(auto const &i : contact_list1) {
            c1.insert(make_pair(i.body1, i.body2));
            c1.insert(make_pair(i.body2, i.body1));
        }

        if(c != c1 || contact_list.size() != contact_list1.size()) {
            cout << c.size() << " " << c1.size() <<  endl;
            cout << "not equal" << endl;
        }*/

#if defined(GLFW_ON)
        world.AddForces(deltaTime);
        //auto contact_list = vector<Contact>();
        world.Update(contact_list, deltaTime);
#else
        world.AddForces(deltaTime.count());
        //auto contact_list = vector<Contact>();
        world.Update(contact_list, deltaTime.count());
#endif


#if defined(GLFW_ON)
        BlockRenderer::DrawAllBlocks(&world.iblocks, &world.zblocks, &world.eblocks, &world.mblocks);
        glfwSwapBuffers(world_properties->window);
        glfwPollEvents();
#endif
        frame_count++;
#if defined(GLFW_ON)
        if(currentFrame - prev_time >= 1.0) {
            cout << "FPS: " << frame_count << endl;
            frame_count = 0;
            prev_time = currentFrame;
        }
#else
        if(duration_cast<milliseconds>(currentFrame - prev_time).count()>= 1000.0) {
            cout << "FPS: " << frame_count << endl;
            frame_count = 0;
            prev_time = currentFrame;
        }
#endif

    }


    cout << "Terminating..." << endl;


#if defined(GLFW_ON)
    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glfwTerminate();
#endif

    cout << "Terminated" << endl;
    return 0;
}