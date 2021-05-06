#include <iostream>
#include <vector>

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
    cout << "Running Prototype 3" << endl;

    WorldProperties *world_properties = WorldIntializer();

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    Camera camera(world_properties->window, Matrix::CreateColumnVec(-30.0f, 0.0f, 5.0f));
    Matrix view = camera.CalculateViewMat();

    Matrix projection = Matrix::Perspective(Matrix::ConvertToRadians(45.0f), 800.0f / 600.0f, 0.1f, 4000.0f);

    real cube_length = 4.0f;
    real position_coord1[] = {-35, -1.0f, -20}; //x, y, z. x is how much horizontal y is vertical. z is in/out
    Matrix position1(3, 1, position_coord1);
    Cube c1(cube_length, position1, Quaternion(1, 0, 0, 0), 1.0f, 1.0f);

    real position_coord2[] = {-5, 0, -20};
    Matrix position2(3, 1, position_coord2);
    Cube c2(cube_length, position2, Quaternion(1, 0, 0, 0), 1.0f, 1.0f);

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

    real initial_momentum1[] = {0.002/4, 0.002/4, 0.0};
    c1.momentum = Matrix(3, 1, initial_momentum1);

    real initial_momentum2[] = {-0.002/4, 0.0015/4, 0};
    c2.momentum = Matrix(3, 1, initial_momentum2);

    Quaternion q = Quaternion(0.0f, 2.0f, 2.0f, 0.0f);
    Quaternion spin = q * c1.orientation;
    c1.orientation += spin;
    c1.orientation.Normalise();

    //real angular_momentum[] = {0.0000002, 0.0002, 0.0000002};
    //c1.angular_momentum = Matrix(3, 1, angular_momentum);

    int frame_count = 0;
    float prev_time = glfwGetTime();

    while (!glfwWindowShouldClose(world_properties->window))
    { // render loop -- an iteration of this main render loop is called a frame

        real currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.UpdateCamera(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*real force_values[] = {30, 0, 0};
        real force_world_coordinate_values[] = {-12, 0, -20};

        Matrix force = Matrix(3, 1, force_values);
        Matrix force_world_coordinate = Matrix(3, 1, force_world_coordinate_values);*/

        //c1.AddTorque(force, force_world_coordinate, 0.1 * deltaTime);
        c1.Update();
        c2.Update();

        vector<Contact> contact_list;
        Cube::CollisionDetect(&c1,&c2, contact_list);

        for(auto & i : contact_list) {
            Cube::CollisionResolution(i);
        }

        Matrix model1 = Matrix(4, 4, 1);
        model1.Translate4by4Matrix(c1.position);
        model1 = Matrix::MatMul(c1.GetOrientationMatrix(), model1); //should be get inverse orientation matrix

        render_utils::CubeRenderer::ApplyUniforms(model1);
        Matrix colour = Matrix::CreateColumnVec(1, 1, 1);
        int colour_loc = glGetUniformLocation(CubeRenderer::shader_id, "colour");
        glUniform3fv(colour_loc, 1, colour.GetValues());

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        Matrix model2 = Matrix(4, 4, 1);
        model2.Translate4by4Matrix(c2.position);
        model2 = Matrix::MatMul(c2.GetOrientationMatrix(), model2);

        view = camera.CalculateViewMat();
        render_utils::CubeRenderer::ApplyUniforms(model2);

        colour_loc = glGetUniformLocation(CubeRenderer::shader_id, "colour");
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