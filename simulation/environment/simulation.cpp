#include <cstring>
#include <unordered_set>
#include <set>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <world_initializer.hpp>
#include <settings.hpp>
#include <matrix.hpp>
#include <camera.hpp>
#include <block_renderer.hpp>
#include <world_handler.hpp>
#include <omp.h>

using namespace std;

using namespace render_utils;
using namespace numerics;
using namespace settings;
using namespace blocks;

int main()
{
    //cout << "Running MWorld Simulation with "  << omp_get_max_threads() << " threads" << endl;
    cout << "Running MWorld Simulation" << endl;

    WorldProperties *world_properties = WorldIntializer();

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    real cube_length = 4.0f;
    Camera camera = Camera(world_properties->window);
    camera.camera_pos = glm::vec3(0, 0, 250);
    BlockRenderer::InitialiseBlockRenderer(&camera, cube_length, vao, vbo, ebo, world_properties);

    int num_blocks_same = 100;
    //WorldHandler world = WorldHandler(num_blocks_same, num_blocks_same, num_blocks_same, num_blocks_same, num_blocks_same, num_blocks_same); //WorldHandler world = WorldHandler(0, 0, 0, 0, 0, 0); //WorldHandler world = WorldHandler(10, 0, 0, 0, 0, 0);
    //WorldHandler world = WorldHandler(0, 0, 2, 0, 0, 0);
    // Test out with different collision elasticity value, so change scaling of velocity after collision, and see how world develops
    WorldHandler world = WorldHandler(0, 0, 100, 0, 00, 0, -100, 100, 4);
    
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

    glBindVertexArray(vao);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUseProgram(world_properties->shader_id);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //using black to clear the background
    std::cout << "Entering Main Loop" << std::endl;

    real deltaTime = 0.0f; // Time between current frame and last frame
    real lastFrame = 0.0f; // Time of last frame
    real currentFrame;

    real frame_count = 0;
    real prev_time = glfwGetTime(); // prev_time refers to the last time we printed the num of frames per seconds
    while (!glfwWindowShouldClose(world_properties->window))
    {

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.UpdateCamera(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //auto contact_list = world.CollisionHandlerSerial();
        //auto contact_list1 = world.CollisionHandlerParallel();

        auto contact_list = world.CollisionHandlerParallel();

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

        if(c != c1) {
            cout << c.size() << " " << c1.size() <<  endl;
            cout << "not equal" << endl;
        }*/


        world.AddForces(deltaTime);
        //auto contact_list = vector<Contact>();
        world.Update(contact_list, deltaTime);

        BlockRenderer::DrawAllBlocks(&world.iblocks, &world.zblocks, &world.eblocks, &world.mblocks);

        glfwSwapBuffers(world_properties->window);
        glfwPollEvents();
        frame_count++;
        if(currentFrame - prev_time >= 1.0) {
            cout << "FPS: " << frame_count << endl;
            frame_count = 0;
            prev_time = currentFrame;
        }
    }

    cout << "Terminating..." << endl;
    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glfwTerminate();
    cout << "Terminated" << endl;
    return 0;
}