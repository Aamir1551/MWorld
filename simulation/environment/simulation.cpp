#include <cstring>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cube_renderer.hpp>
#include <world_initializer.hpp>
#include <settings.hpp>
#include <matrix.hpp>
#include <camera.hpp>
#include <block_renderer.hpp>
#include <world_handler.cpp>

using namespace std;

using namespace render_utils;
using namespace numerics;
using namespace settings;
using namespace blocks;

// TODO
// 1) Add font colour to terminal screen whenever prototype_one or two or three are running.
// Add a different font colour for each of them in the terminal.
// So that it is more clearer to know which prototype is running.

int main()
{
    cout << "Running MWorld Simulation" << endl;

    WorldProperties *world_properties = world_intializer();

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    real cube_length = 4.0f;
    BlockRenderer::InitialiseBlockRenderer(camera, cube_length, vao, vbo, ebo, world_properties);

    int num_blocks_same = 30;
    WorldHandler world = WorldHandler(num_blocks_same, num_blocks_same, num_blocks_same, num_blocks_same, num_blocks_same, num_blocks_same);
    //WorldHandler world = WorldHandler(0, 0, 0, 0, 0, 0);
    //WorldHandler world = WorldHandler(0, 0, 2, 0, 0, 0);
    //WorldHandler world = WorldHandler(0, 0, 160, 0, 0, 0);

    CubeRenderer::InitializeCubes(cube_length, vao, vbo, ebo, &view, &projection, world_properties->shader_id);
    CubeRenderer::AddVerticesToBuffers();

    glBindVertexArray(vao);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUseProgram(world_properties->shader_id);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //using black to clear the background
    std::cout << "Entering Main Loop" << std::endl;

    real deltaTime = 0.0f; // Time between current frame and last frame
    real lastFrame = 0.0f; // Time of last frame
    real currentFrame;

    /*real initial_momentum1[] = {0.002, 0, 0};
    world.iblocks.at(0).momentum = Matrix(3, 1, initial_momentum1);

    real initial_momentum2[] = {-0.002, 0.0, 0};
    world.iblocks.at(1).momentum = Matrix(3, 1, initial_momentum2);*/

    /*Quaternion q = Quaternion(0.0f, 0.0f, 1.7f, -1.7f);
    Quaternion spin = q * world.iblocks.at(0).orientation;
    world.iblocks.at(0).orientation += spin;
    world.iblocks.at(0).orientation.Normalise();*/

    //world.iblocks.at(0).angular_momentum = Matrix(3, 1);
    //world.iblocks.at(1).angular_momentum = Matrix(3, 1);

    real frame_count = 0;
    real prev_time = glfwGetTime();
    while (!glfwWindowShouldClose(world_properties->window))
    {

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.UpdateCamera(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world.Update();
        world.CollisionHandler();
        world.AddForces();

        DrawAllBlocks(world, id, camera, view);

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

