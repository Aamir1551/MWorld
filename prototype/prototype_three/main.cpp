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
    Camera camera(world_properties->window, glm::vec3(-30.0f, 0.0f, 5.0f));
    glm::mat4 view = camera.CalculateView();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 4000.0f);

    real cube_length = 4.0f;
    real position_coord1[] = {-35, -2.0f, -20}; //x, y, z. x is how much horizontal y is vertical. z is in/out
    Matrix position1(3, 1, position_coord1);
    Cube c1(cube_length, position1, Quaternion(1, 0, 0, 0), 1.0f, 1.0f);

    real position_coord2[] = {-25, 0, -20};
    Matrix position2(3, 1, position_coord2);
    Cube c2(cube_length, position2, Quaternion(1, 0, 0, 0), 1.0f, 1.0f);

    CubeRenderer::InitializeCubes(cube_length, vao, vbo, ebo, &view, &projection, world_properties->shader_id);
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

    real initial_momentum1[] = {0.002, 0.0, 0.0};
    c1.momentum = Matrix(3, 1, initial_momentum1);

    real initial_momentum2[] = {-0.002, 0.0, 0};
    c2.momentum = Matrix(3, 1, initial_momentum2);

    Quaternion q = Quaternion(0.0f, 2.0f, 2.0f, 0.0f);
    Quaternion spin = q * c1.orientation;
    c1.orientation += spin;
    c1.orientation.Normalise();

    //real angular_momentum[] = {0.0000002, 0.0002, 0.0000002};
    //c1.angular_momentum = Matrix(3, 1, angular_momentum);

    bool paused = false;

    while (!glfwWindowShouldClose(world_properties->window))
    { // render loop -- an iteration of this main render loop is called a frame

        /*if (glfwGetKey(world_properties->window, GLFW_KEY_P) == GLFW_PRESS) {
           paused = true;
        }
        cout << paused << endl;

        while(paused) {
            cout << "in pause" << endl;
            if (glfwGetKey(world_properties->window, GLFW_KEY_A) == GLFW_PRESS) {
                cout << "if" << endl;
                paused = false;
            }
        }*/

        real currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.UpdateCamera(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        real amount = 0.0001;

        /*real force_values[] = {30, 0, 0};
        real force_world_coordinate_values[] = {-12, 0, -20};

        Matrix force = Matrix(3, 1, force_values);
        Matrix force_world_coordinate = Matrix(3, 1, force_world_coordinate_values);*/

        //c1.AddTorque(force, force_world_coordinate, 0.1 * deltaTime);
        c1.Update();
        c2.Update();

        vector<Contact> contact_list;
        Cube::CollisionDetect(&c1,&c2, contact_list);

        for(int i=0; i<contact_list.size(); i++) {
            Cube::CollisionResolution(contact_list.at(i));
        }

        glm::mat4 rotation_mat1;
        memcpy(glm::value_ptr(rotation_mat1), c1.GetOrientationMatrix().GetValues(), 16 * sizeof(real));

        glm::vec3 translation_mat1;
        memcpy(glm::value_ptr(translation_mat1), c1.position.GetValues(), 3 * sizeof(real));

        glm::mat4 model1 = glm::translate(id, translation_mat1);
        model1 = model1 * rotation_mat1;

        cubes.ApplyUniforms(model1);
        glm::vec3 colour = glm::vec3(1, 1, 1);
        int colour_loc = glGetUniformLocation(CubeRenderer::shader_id, "colour");
        glUniform3fv(colour_loc, 1, glm::value_ptr(colour));

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 rotation_mat2;
        memcpy(glm::value_ptr(rotation_mat2), c2.GetOrientationMatrix().GetValues(), 16 * sizeof(real));

        glm::vec3 translation_mat2;
        memcpy(glm::value_ptr(translation_mat2), c2.position.GetValues(), 3 * sizeof(real));

        glm::mat4 model2 = glm::translate(id, translation_mat2);
        model2 = model2 * rotation_mat2;

        view = camera.CalculateView();
        cubes.ApplyUniforms(model2);

        colour_loc = glGetUniformLocation(CubeRenderer::shader_id, "colour");
        glUniform3fv(colour_loc, 1, glm::value_ptr(colour));

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

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