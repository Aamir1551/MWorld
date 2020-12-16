#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <cstdlib> //for the rand function
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
#include <quaternion.hpp>
#include <camera.hpp>
#include <cube.cpp>

using namespace std;

using namespace render_utils;
using namespace numerics;
using namespace settings;

// TODO
// 1) Add font colour to terminal screen whenever prototype_one or two or three are running.
// Add a different font colour for each of them in the terminal.
// So that it is more clearer to know which prototype is running.

int main()
{
    cout << "Running Prototype 3" << endl;

    WorldProperties *world_properties = world_intializer();

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glm::mat4 id = glm::mat4(1.0f);
    Camera camera(world_properties->window);
    glm::mat4 view = camera.CalculateView();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 400.0f);

    real position_coord1[] = {0, 0, -10};
    Matrix position1(3, 1, position_coord1);
    Cube c1(position1, Quaternion(1, 0, 0, 0), 0.0000000005f);

    real position_coord2[] = {10, 0, -10};
    Matrix position2(3, 1, position_coord2);
    Cube c2(position2, Quaternion(1, 0, 0, 0), 0.0000000005f);

    CubeRenderer::InitializeCubes(4.0f, vao, vbo, ebo, &view, &projection, world_properties->shader_id);
    CubeRenderer::AddVerticesToBuffers();

    CubeRenderer cubes;

    glBindVertexArray(vao);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUseProgram(world_properties->shader_id);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //using black to clear the background
    std::cout << "Entering Main Loop" << std::endl;

    real deltaTime = 0.0f; // Time between current frame and last frame
    real lastFrame = 0.0f; // Time of last frame

    while (!glfwWindowShouldClose(world_properties->window))
    { // render loop -- an iteration of this main render loop is called a frame

        real currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.UpdateCamera(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        real amount = 0.0001;

        c1.Update();
        c2.Update();

        glm::mat4 rotation_mat1;
        memcpy(glm::value_ptr(rotation_mat1), c1.GetOrientationMatrix().GetValues(), 16 * sizeof(real));

        glm::vec3 translation_mat1;
        memcpy(glm::value_ptr(translation_mat1), c1.position.GetValues(), 3 * sizeof(real));

        glm::mat4 model1 = glm::translate(id, translation_mat1);
        model1 = model1 * rotation_mat1;
        cubes.models.push_back(&model1);

        cubes.ApplyUniforms(0);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 rotation_mat2;
        memcpy(glm::value_ptr(rotation_mat2), c2.GetOrientationMatrix().GetValues(), 16 * sizeof(real));

        glm::vec3 translation_mat2;
        memcpy(glm::value_ptr(translation_mat2), c2.position.GetValues(), 3 * sizeof(real));

        glm::mat4 model2 = glm::translate(id, translation_mat2);
        model2 = model2 * rotation_mat2;
        cubes.models.push_back(&model2);

        view = camera.CalculateView();
        cubes.ApplyUniforms(1);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        cubes.models.clear();

        glfwSwapBuffers(world_properties->window);
        glfwPollEvents();
    }

    cout << "Terminating..." << endl;
    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glfwTerminate();
    cout << "Terminated" << endl;
    return 0;
}