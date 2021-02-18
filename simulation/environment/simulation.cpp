#include <cstring>

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

using namespace std;

using namespace render_utils;
using namespace numerics;
using namespace settings;
using namespace blocks;

int main()
{
    cout << "Running MWorld Simulation" << endl;

    WorldProperties *world_properties = world_intializer();

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    real cube_length = 4.0f;
    Camera camera = Camera(world_properties->window);
    camera.camera_pos = glm::vec3(0, 0, 250);
    BlockRenderer::InitialiseBlockRenderer(&camera, cube_length, vao, vbo, ebo, world_properties);

    int num_blocks_same = 50;
    //WorldHandler world = WorldHandler(num_blocks_same, num_blocks_same, num_blocks_same, num_blocks_same, num_blocks_same, num_blocks_same);
    //WorldHandler world = WorldHandler(0, 0, 0, 0, 0, 0);
    //WorldHandler world = WorldHandler(10, 0, 0, 0, 0, 0);
    //WorldHandler world = WorldHandler(0, 0, 2, 0, 0, 0);
    WorldHandler world = WorldHandler(0, 0, 500, 0, 00, 0, -100, 100, 4);
    /*world.forces_tree->RemoveZBlock(world.zblocks.at(0));
    world.forces_tree->RemoveZBlock(world.zblocks.at(1));
    world.tree->RemoveZBlock(world.zblocks.at(0));
    world.tree->RemoveZBlock(world.zblocks.at(1));

    world.zblocks.at(0)->position = Matrix::CreateColumnVec(-20, 0, 0);
    world.zblocks.at(1)->position = Matrix::CreateColumnVec(20, 0, 0);

    world.tree->AddZBlock(world.zblocks.at(0));
    world.tree->AddZBlock(world.zblocks.at(1));
    world.forces_tree->AddZBlock(world.zblocks.at(0));
    world.forces_tree->AddZBlock(world.zblocks.at(1));

    world.zblocks.at(0)->SetLinearMomentumToZero();
    world.zblocks.at(0)->momentum = Matrix(3, 1);
    world.zblocks.at(1)->SetLinearMomentumToZero();
    world.zblocks.at(1)->momentum = Matrix(3, 1);*/


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
    real prev_time = glfwGetTime();
    while (!glfwWindowShouldClose(world_properties->window))
    {

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.UpdateCamera(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto contact_list = world.CollisionHandler();
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