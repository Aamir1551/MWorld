#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <cstdlib> //for the rand function

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cubes_collection_render.hpp>
#include <world_initializer.hpp>
#include <settings.h>
#include <matrix.h>
#include <quaternion.hpp>
#include <camera.hpp>
#include "cube.cpp"
#include <cstring>

using namespace std;
//TODO
//1) set render/lib -> lib in root directory
//2) set up debugging in vscode

std::vector<glm::vec3> *GeneratePosition(int num_cubes);
std::vector<glm::vec3> *GenerateRotationsAxis(int num_cubes);

int main()
{
    cout << "Running Prototype 1" << endl;
    settings::real coordinates[] = {0, 0, 10}; //world coordinates of the cube
    settings::real left_force_coordinates[] = {-1, 0, 10};
    settings::real right_force_coordinates[] = {1, 0, 10};

    Matrix world_coordinates = Matrix(3, 1, coordinates);
    Matrix right_force = Matrix(3, 1, right_force_coordinates);
    Matrix left_force = Matrix(3, 1, left_force_coordinates);

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

    CubesCollection::InitializeCubes(4.0f, vao, vbo, ebo, &view, &projection, world_properties->shader_id);
    CubesCollection::AddVerticesToBuffers();

    CubesCollection cubes;

    glBindVertexArray(vao);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUseProgram(world_properties->shader_id);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //using black to clear the background
    std::cout << "Entering Main Loop" << std::endl;

    float deltaTime = 0.0f; // Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    c.add_torque(right_force, world_coordinates, 0.0005); //put this in loop after
    while (!glfwWindowShouldClose(world_properties->window))
    { // render loop -- an iteration of this main render loop is called a frame

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.UpdateCamera(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(world_properties->window, GLFW_KEY_J) == GLFW_PRESS)
            c.add_torque(left_force, world_coordinates, 0.0000001);
        if (glfwGetKey(world_properties->window, GLFW_KEY_L) == GLFW_PRESS)
            c.add_torque(right_force, world_coordinates, 0.0000001);

        for (int i = 0; i < num_cubes; i++)
        {

            glm::mat4 model = glm::translate(id, positions.at(i));

            glm::mat4 temp;
            memcpy(glm::value_ptr(temp), Quaternion::GetMatrixTransformation(c.orientation).getValues(), 16 * 4);
            model = model * temp;

            //model = glm::rotate(model, glm::radians((float)glfwGetTime() * 20 * ((i + 1) % 20)), rotations->at(i));
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

std::vector<glm::vec3> *GeneratePosition(int num_cubes)
{
    srand((unsigned)time(NULL)); //NULL???
    std::vector<glm::vec3> *positions = new std::vector<glm::vec3>;
    float world_size = 500;
    float const scale = world_size / ((float)RAND_MAX / 2.0);
    auto get_coord = [scale, world_size]() -> float { return scale * (rand() - RAND_MAX / 2); };
    for (int i = 0; i < num_cubes; i++)
    {
        positions->push_back(glm::vec3(get_coord(), get_coord(), get_coord()));
    }
    return positions;
}

std::vector<glm::vec3> *GenerateRotationsAxis(int num_cubes)
{
    srand((unsigned)time(NULL)); //NULL???
    std::vector<glm::vec3> *rotations = new std::vector<glm::vec3>;
    for (int i = 0; i < num_cubes; i++)
    {
        rotations->push_back(glm::vec3(rand() % 5 - 2, rand() % 5 - 2, rand() % 5 - 2)); //try this without a pointer to see if it'l work.
    }
    return rotations;
}