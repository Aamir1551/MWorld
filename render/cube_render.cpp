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
#include <camera.hpp>
#include <world_initializer.hpp>

std::vector<glm::vec3> *GeneratePosition(int num_cubes);
std::vector<glm::vec3> *GenerateRotationsAxis(int num_cubes);

// TODO:
// 1) Go over mouse controls and understand properly how they work, also how cameraFront works
// so read again from page 97
// 2) Organise code layout properly, and sort out cmakelists files for the full render directory
// 3) Please get notion
// 4) read over random numbers in c++
// 5) statics and linking and why static variables should be in header file only, and why u'll get linking errors if you don't
// 6) Learn to add a colour using element buffer objects
// 7) Learn about cmake install and export and apply it to project if its useful
int main()
{

    WorldProperties *world_properties = world_intializer();

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glm::mat4 id = glm::mat4(1.0f);
    Camera camera(world_properties->window);
    glm::mat4 view = camera.CalculateView();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 400.0f);

    int num_cubes = 3000;
    std::vector<glm::vec3> *positions = GeneratePosition(num_cubes);
    std::vector<glm::vec3> *rotations = GenerateRotationsAxis(num_cubes);

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

    while (!glfwWindowShouldClose(world_properties->window))
    { // render loop -- an iteration of this main render loop is called a frame

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.UpdateCamera(deltaTime);
        //glfwSetCursorPosCallback(world_properties->window, mouse_callback);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < num_cubes; i++)
        {
            glm::mat4 model = glm::translate(id, positions->at(i));
            model = glm::rotate(model, glm::radians((float)glfwGetTime() * 20 * ((i + 1) % 20)), rotations->at(i));
            cubes.models.push_back(&model);

            //view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            view = camera.CalculateView();

            cubes.ApplyUniforms(i);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        cubes.models.clear();

        glfwSwapBuffers(world_properties->window);
        glfwPollEvents(); //checks if any events are triggered, and calls their respective handlers
    }

    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glfwTerminate();
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
