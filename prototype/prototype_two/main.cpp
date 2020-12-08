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
    cout << "Running Prototype 2" << endl;

    //world coordinats of forces
    real left_force_coordinates[] = {-2, 0, -8};
    real right_force_coordinates[] = {2, 0, -8};
    real up_force_coordinates[] = {0, 2, -8};
    real down_force_coordinates[] = {0, -2, -8};
    real force_vector[] = {0, 0, -5};

    Matrix force_world_vector(3, 1, force_vector);
    Matrix right_force(3, 1, right_force_coordinates);
    Matrix left_force(3, 1, left_force_coordinates);
    Matrix up_force(3, 1, up_force_coordinates);
    Matrix down_force(3, 1, down_force_coordinates);

    WorldProperties *world_properties = world_intializer();

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glm::mat4 id = glm::mat4(1.0f);
    Camera camera(world_properties->window);
    glm::mat4 view = camera.CalculateView();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 400.0f);

    real position_coord[] = {0, 0, -10};
    Matrix position(3, 1, position_coord);
    Cube c(position);

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
        if (glfwGetKey(world_properties->window, GLFW_KEY_J) == GLFW_PRESS)
            c.AddTorque(force_world_vector, left_force, amount * deltaTime);
        if (glfwGetKey(world_properties->window, GLFW_KEY_L) == GLFW_PRESS)
            c.AddTorque(force_world_vector, right_force, amount * deltaTime);

        if (glfwGetKey(world_properties->window, GLFW_KEY_I) == GLFW_PRESS)
            c.AddTorque(force_world_vector, up_force, amount * deltaTime);
        if (glfwGetKey(world_properties->window, GLFW_KEY_K) == GLFW_PRESS)
            c.AddTorque(force_world_vector, down_force, amount * deltaTime);

        if (glfwGetKey(world_properties->window, GLFW_KEY_SPACE) == GLFW_PRESS)
            c.SetAngularMomentumToZero();

        c.Update();
        glm::mat4 rotation_mat;
        memcpy(glm::value_ptr(rotation_mat), c.GetOrientationMatrix().GetValues(), 16 * sizeof(real));

        glm::vec3 translation_mat;
        memcpy(glm::value_ptr(translation_mat), c.position.GetValues(), 3 * sizeof(real));

        glm::mat4 model = glm::translate(id, translation_mat);
        model = model * rotation_mat;
        cubes.models.push_back(&model);
        view = camera.CalculateView();
        cubes.ApplyUniforms(0);

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