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
#include <camera.hpp>
#include <world_handler.cpp>

using namespace std;

using namespace render_utils;
using namespace numerics;
using namespace settings;
using namespace blocks;

// TODO
// 1) Add font colour to terminal screen whenever prototype_one or two or three are running.
// Add a different font colour for each of them in the terminal.
// So that it is more clearer to know which prototype is running.

void DrawBlocks(vector<Block *> *block_list,glm::vec3 colour, glm::mat4& id, CubeRenderer &cubes, Camera &camera, glm::mat4 &view);

int main()
{
    cout << "Running MWorld Simulation" << endl;

    WorldProperties *world_properties = world_intializer();

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glm::mat4 id = glm::mat4(1.0f);
    Camera camera(world_properties->window);
    glm::mat4 view = camera.CalculateView();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 400.0f);

    real cube_length = 4.0f;

    WorldHandler world = WorldHandler(0, 1, 0, 0, 0, 0);

    real position_coord1[] = {-20, -2.0f, -20}; //x, y, z. x is how much horizontal y is vertical. z is in/out
    Matrix position1(3, 1, position_coord1);
    world.iblocks.at(0)->position =  position1;
    world.iblocks.at(0)->flare_value = 10;

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
    real currentFrame;

    real frame_count = 0;
    real prev_time = glfwGetTime();
    while (!glfwWindowShouldClose(world_properties->window))
    {

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.UpdateCamera(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world.Update();
        world.CollisionHandler();
        world.AddForces();

        DrawBlocks( (vector<Block*> *) &(world.iblocks), glm::vec3(0, 1, 1), id, cubes, camera, view);
        DrawBlocks( (vector<Block*> *) &(world.zblocks), glm::vec3(1, 0, 1), id, cubes, camera, view);
        DrawBlocks( (vector<Block*> *) &(world.eblocks), glm::vec3(0, 0, 1), id, cubes, camera, view);
        DrawBlocks( (vector<Block*> *) &(world.mblocks), glm::vec3(1, 1, 1), id, cubes, camera, view);

        glfwSwapBuffers(world_properties->window);
        glfwPollEvents();
        frame_count++;
        if(currentFrame - prev_time >= 1.0) {
            cout << "FPS: " << frame_count << endl;
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

void DrawBlocks(vector<Block *> *block_list,glm::vec3 colour, glm::mat4& id, CubeRenderer &cubes, Camera &camera, glm::mat4 &view) {
    for(auto & block_ptr : *block_list) {
        glm::mat4 rotation_mat;
        memcpy(glm::value_ptr(rotation_mat), block_ptr->GetOrientationMatrix().GetValues(), 16 * sizeof(real));

        glm::vec3 translation_mat;
        memcpy(glm::value_ptr(translation_mat), block_ptr->position.GetValues(), 3 * sizeof(real));

        glm::mat4 model = glm::translate(id, translation_mat);
        model = model * rotation_mat;
        cubes.ApplyUniforms(model);

        int colour_loc = glGetUniformLocation(CubeRenderer::shader_id, "colour");
        glUniform3fv(colour_loc, 1, glm::value_ptr(colour));
        view = camera.CalculateView();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}
