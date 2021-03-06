#include <cstring>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
    cout << "Running Logic Gates Simulation" << endl;

    WorldProperties *world_properties = WorldIntializer();

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    real cube_length = 4.0f;
    Camera camera = Camera(world_properties->window);
    camera.camera_pos_mat =  Matrix::CreateColumnVec(-20, 0, 20);
    BlockRenderer::InitialiseBlockRenderer(&camera, cube_length, vao, vbo, ebo, world_properties);

    real down = -10;

    // Not Gate -- Given by 1 Input, Input 1 (A) = I block, Out (C) = M block :
    // I+
    // M (A)
    // Z (T1)
    // I- (T2)
    // M (M1)
    // M (M2)
    // M (C)
    WorldHandler world = WorldHandler(1, 1, 1, 4, 0, 0);

    // I+
    world.iblocks.at(0)->SetLinearMomentumToZero();
    world.iblocks.at(0)->position = Matrix::CreateColumnVec(-22, down, -13);
    world.iblocks.at(0)->locked = true;

    // M0
    world.mblocks.at(0)->SetLinearMomentumToZero();
    world.mblocks.at(0)->position = Matrix::CreateColumnVec(-22+4, down, -13);
    world.mblocks.at(0)->locked = true;

    // Z
    world.zblocks.at(0)->SetLinearMomentumToZero();
    world.zblocks.at(0)->position = Matrix::CreateColumnVec(-22+8, down, -13);
    world.zblocks.at(0)->locked = true;

    // I-
    world.iblocks.at(1)->SetLinearMomentumToZero();
    world.iblocks.at(1)->position = Matrix::CreateColumnVec(-22+12, down, -13);
    world.iblocks.at(1)->locked = true;

    // M1
    world.mblocks.at(1)->SetLinearMomentumToZero();
    world.mblocks.at(1)->position = Matrix::CreateColumnVec(-22+16, down, -13);
    world.mblocks.at(1)->locked = true;

    // M2
    world.mblocks.at(2)->SetLinearMomentumToZero();
    world.mblocks.at(2)->position = Matrix::CreateColumnVec(-22+20, down, -13);
    world.mblocks.at(2)->locked = true;

    // Out
    world.mblocks.at(3)->SetLinearMomentumToZero();
    world.mblocks.at(3)->position = Matrix::CreateColumnVec(-22+24, down, -13);
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

        if (glfwGetKey(world_properties->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            is_out = !is_out;
            if (is_out) {
                world.iblocks.at(0)->position = Matrix::CreateColumnVec(-28, down, -13);
            } else {
                world.iblocks.at(0)->position = Matrix::CreateColumnVec(-22, down, -13);
            }
            world.ResetTrees();
        }


        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.UpdateCamera(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto contact_list = world.CollisionHandlerBruteForce();
        world.AddForcesViaBarnesHut(deltaTime);
        world.Update(contact_list, deltaTime);

        BlockRenderer::DrawAllBlocks(&world.iblocks, &world.zblocks, &world.eblocks, &world.mblocks);

        glfwSwapBuffers(world_properties->window);
        glfwPollEvents();
        frame_count++;
        if(currentFrame - prev_time >= 1.0) {
            cout << "FPS: " << frame_count << endl;
            frame_count = 0;
            prev_time = currentFrame;

            // NOT Gate
            cout << "Flare value in M (A) block: " << world.mblocks.at(0)->flare_value << endl;
            cout << "Flare value in Z (T1) block: " << world.zblocks.at(0)->flare_value << endl;
            cout << "Flare value in I- (T2) block: " << world.iblocks.at(1)->flare_value << endl;
            cout << "Flare value in M (M1) block: " << world.mblocks.at(1)->flare_value << endl;
            cout << "Flare value in M (M2) block: " << world.mblocks.at(2)->flare_value << endl;
            cout << "Flare value in C (M3) block: " << world.mblocks.at(3)->flare_value << endl;
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