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
//TODO
//1) set render/lib -> lib in root directory
//2) set up debugging in vscode

using namespace render_utils;
using namespace numerics;

int main()
{
    cout << "Running Prototype 1" << endl;
    settings::real coordinates[] = {0, 0, -10};             //world coordinates of the cube
    settings::real left_force_coordinates[] = {-2, 0, -10}; //world coordinats of forces
    settings::real right_force_coordinates[] = {2, 0, -10};
    settings::real up_force_coordinates[] = {0, 2, -10};
    settings::real down_force_coordinates[] = {0, -2, -10};
    settings::real fff[] = {0, 0, -1};

    Matrix world_coordinates = Matrix(3, 1, coordinates);
    Matrix fff_force = Matrix(3, 1, fff);
    Matrix right_force = Matrix(3, 1, right_force_coordinates);
    Matrix left_force = Matrix(3, 1, left_force_coordinates);
    Matrix up_force = Matrix(3, 1, up_force_coordinates);
    Matrix down_force = Matrix(3, 1, down_force_coordinates);

    WorldProperties *world_properties = world_intializer();

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glm::mat4 id = glm::mat4(1.0f);
    Camera camera(world_properties->window);
    glm::mat4 view = camera.CalculateView();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 400.0f);

    int num_cubes = 1;
    float position_coord[] = {0, 0, -10};
    Matrix position(3, 1, position_coord);
    Cube c(position);
    std::vector<glm::vec3> positions = {glm::vec3(0, 0, -10)};

    std::vector<Matrix> rotations = {Quaternion::GetMatrixTransformation(c.orientation)};

    CubeRenderer::InitializeCubes(4.0f, vao, vbo, ebo, &view, &projection, world_properties->shader_id);
    CubeRenderer::AddVerticesToBuffers();

    CubeRenderer cubes;

    glBindVertexArray(vao);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUseProgram(world_properties->shader_id);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //using black to clear the background
    std::cout << "Entering Main Loop" << std::endl;

    float deltaTime = 0.0f; // Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    while (!glfwWindowShouldClose(world_properties->window))
    { // render loop -- an iteration of this main render loop is called a frame

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.UpdateCamera(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float amount = 0.0001;
        if (glfwGetKey(world_properties->window, GLFW_KEY_J) == GLFW_PRESS)
            c.add_torque(fff_force, left_force, amount * deltaTime);
        if (glfwGetKey(world_properties->window, GLFW_KEY_L) == GLFW_PRESS)
            c.add_torque(fff_force, right_force, amount * deltaTime);

        if (glfwGetKey(world_properties->window, GLFW_KEY_I) == GLFW_PRESS)
            c.add_torque(fff_force, up_force, amount * deltaTime);
        if (glfwGetKey(world_properties->window, GLFW_KEY_K) == GLFW_PRESS)
            c.add_torque(fff_force, down_force, amount * deltaTime);

        c.Update();
        for (int i = 0; i < num_cubes; i++)
        {

            glm::mat4 model = glm::translate(id, positions.at(i));
            glm::mat4 temp;

            memcpy(glm::value_ptr(temp), Quaternion::GetMatrixTransformation(c.orientation).getValues(), 16 * 4);
            model = model * temp;

            cubes.models.push_back(&model);

            view = camera.CalculateView();

            cubes.ApplyUniforms(i);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        cubes.models.clear();

        glfwSwapBuffers(world_properties->window);
        glfwPollEvents(); //checks if any events are triggered, and calls their respective handlers
    }

    cout << "Terminating..." << endl;
    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glfwTerminate();
    cout << "Terminated" << endl;
    return 0;
}