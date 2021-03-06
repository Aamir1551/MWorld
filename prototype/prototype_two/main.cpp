#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cube_renderer.hpp>
#include <world_initializer.hpp>
#include <settings.hpp>
#include <matrix.hpp>
#include <quaternion.hpp>
#include <camera.hpp>
#include <cube.cpp>

using namespace std;

using namespace render_utils;
using namespace numerics;
using namespace settings;

int main()
{
    cout << "Running Prototype 2" << endl;

    //world coordinats of forces
    real left_force_coordinates[] = {-2, 0, -5};
    real right_force_coordinates[] = {2, 0, -5};
    real up_force_coordinates[] = {0, 2, -5};
    real down_force_coordinates[] = {0, -2, -5};
    real force_vector[] = {0, 0, -5}; // make sure to change direction of force

    Matrix force_world_vector(3, 1, force_vector);

    Matrix right_force(3, 1, right_force_coordinates);
    Matrix left_force(3, 1, left_force_coordinates);
    Matrix up_force(3, 1, up_force_coordinates);
    Matrix down_force(3, 1, down_force_coordinates);

    WorldProperties *world_properties = WorldIntializer();

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    Camera camera(world_properties->window);
    Matrix view = camera.CalculateViewMat();

    Matrix projection = Matrix::Perspective(Matrix::ConvertToRadians(45.0f), 800.0f / 600.0f, 0.1f, 400.0f);

    real position_coord[] = {0, 0, -10};
    Matrix position(3, 1, position_coord);
    Cube c(position, Quaternion(1, 0, 0, 0), 0.5f);

    CubeRenderer::InitializeCubes(4.0f, vao, vbo, ebo, &view, &projection, world_properties->shader_id);
    CubeRenderer::AddVerticesToBuffers();

    glBindVertexArray(vao);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUseProgram(world_properties->shader_id);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //using black to clear the background
    std::cout << "Entering Main Loop" << std::endl;

    real deltaTime = 0.0f; // Time between current frame and last frame
    real lastFrame = 0.0f; // Time of last frame

    up_force += c.position;
    down_force += c.position;
    right_force += c.position;
    left_force += c.position;

    int frame_count = 0;
    float prev_time = glfwGetTime();

    while (!glfwWindowShouldClose(world_properties->window))
    { // render loop -- an iteration of this main render loop is called a frame

        real currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.UpdateCamera(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        real amount = 0.0001;
        if (glfwGetKey(world_properties->window, GLFW_KEY_J) == GLFW_PRESS)
            c.AddTorque(force_world_vector, left_force, amount * deltaTime);
        if (glfwGetKey(world_properties->window, GLFW_KEY_L) == GLFW_PRESS)
            c.AddTorque(force_world_vector, right_force,amount * deltaTime);

        if (glfwGetKey(world_properties->window, GLFW_KEY_I) == GLFW_PRESS)
            c.AddTorque(force_world_vector,  up_force, amount * deltaTime);
        if (glfwGetKey(world_properties->window, GLFW_KEY_K) == GLFW_PRESS)
            c.AddTorque(force_world_vector,  down_force, amount * deltaTime);

        if (glfwGetKey(world_properties->window, GLFW_KEY_SPACE) == GLFW_PRESS)
            c.SetAngularMomentumToZero();

        c.Update();

        Matrix model = Matrix::CreateTranslationMatrix(c.position);
        model = Matrix::MatMul(c.GetInverseOrientationMatrix(), model);
        view = camera.CalculateViewMat();
        CubeRenderer::ApplyUniforms(model);

        Matrix colour = Matrix::CreateColumnVec(1, 1, 1);
        int colour_loc = glGetUniformLocation(CubeRenderer::shader_id, "colour");
        glUniform3fv(colour_loc, 1, colour.GetValues());

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(world_properties->window);
        glfwPollEvents();

        frame_count++;
        if(currentFrame - prev_time >= 1.0) {
            std::cout << "FPS: " << frame_count << std::endl;
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