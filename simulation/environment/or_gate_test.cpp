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
    cout << "Running Or Gate Simulation" << endl;

    WorldProperties *world_properties = WorldIntializer();

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    real cube_length = 4.0f;
    Camera camera = Camera(world_properties->window);
    camera.camera_pos =  glm::vec3(-20, 0, 20);
    BlockRenderer::InitialiseBlockRenderer(&camera, cube_length, vao, vbo, ebo, world_properties);

    real down = -10;

    // Or Gate -- Given by 2 Inputs, Input 1 (A) = M block, Input 2 (B) = M block, Out (C) = M block :
    //       M(C)
    // M(A)  M0  M(B)
    // I1         I2
    WorldHandler world = WorldHandler(2, 0, 0, 4, 0, 0);

    // I1
    world.iblocks.at(0)->SetLinearMomentumToZero();
    world.iblocks.at(0)->position = Matrix::CreateColumnVec(-22, down, -13);
    world.iblocks.at(0)->locked = true;

    // I2
    world.iblocks.at(1)->SetLinearMomentumToZero();
    world.iblocks.at(1)->position = Matrix::CreateColumnVec(-22, down, -13+8);
    world.iblocks.at(1)->locked = true;

    // A
    world.mblocks.at(0)->SetLinearMomentumToZero();
    world.mblocks.at(0)->position = Matrix::CreateColumnVec(-22 + 4, down, -13);
    world.mblocks.at(0)->locked = true;

    // B
    world.mblocks.at(1)->SetLinearMomentumToZero();
    world.mblocks.at(1)->position = Matrix::CreateColumnVec(-22 + 4, down, -13+8);
    world.mblocks.at(1)->locked = true;

    // M0
    world.mblocks.at(2)->SetLinearMomentumToZero();
    world.mblocks.at(2)->position = Matrix::CreateColumnVec(-22 + 4, down, -13+4);
    world.mblocks.at(2)->locked = true;

    // C
    world.mblocks.at(3)->SetLinearMomentumToZero();
    world.mblocks.at(3)->position = Matrix::CreateColumnVec(-22 + 8, down, -13+4);
    world.mblocks.at(3)->locked = true;

    world.ResetTrees();

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
    int is_out = false;
    while (!glfwWindowShouldClose(world_properties->window))
    {

        if (glfwGetKey(world_properties->window, GLFW_KEY_J) == GLFW_PRESS) {
            is_out = !is_out;
            if (is_out) {
                world.iblocks.at(0)->position = Matrix::CreateColumnVec(-28, down, -13);
            } else {
                world.iblocks.at(0)->position = Matrix::CreateColumnVec(-22, down, -13);
            }
            world.ResetTrees();
        }

        if (glfwGetKey(world_properties->window, GLFW_KEY_K) == GLFW_PRESS) {
            is_out = !is_out;
            if (is_out) {
                world.iblocks.at(1)->position = Matrix::CreateColumnVec(-28, down, -13 + 8);
            } else {
                world.iblocks.at(1)->position = Matrix::CreateColumnVec(-22, down, -13 + 8);
            }
            world.ResetTrees();
        }

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.UpdateCamera(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto contact_list = world.CollisionHandlerLoop();
        world.AddForces(deltaTime);
        world.Update(contact_list, deltaTime);

        BlockRenderer::DrawAllBlocks(&world.iblocks, &world.zblocks, &world.eblocks, &world.mblocks);

        glfwSwapBuffers(world_properties->window);
        glfwPollEvents();
        frame_count++;
        if(currentFrame - prev_time >= 1.0) {
            cout << "FPS: " << frame_count << endl;
            frame_count = 0;
            prev_time = currentFrame;

            // Or Gate
            cout << "Flare value in A block: " << world.mblocks.at(0)->flare_value << endl;
            cout << "Flare value in B block: " << world.mblocks.at(1)->flare_value << endl;
            cout << "Flare value in C block: " << world.mblocks.at(3)->flare_value << endl;
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