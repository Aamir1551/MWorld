#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib> //for the rand function

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cube_renderer.hpp>
#include <camera.hpp>
#include <world_initializer.hpp>
#include <matrix.hpp>
#include <quaternion.hpp>

using namespace render_utils;
using namespace numerics;
using namespace std;

std::vector<Matrix> *GeneratePosition(int num_cubes);
std::vector<Quaternion> *GenerateRotationsAxis(int num_cubes);

int main()
{

    std::cout << "Running prototype 1" << std::endl;
    WorldProperties *world_properties = WorldIntializer();

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    Camera camera(world_properties->window);
    Matrix view_mat = camera.CalculateViewMat();

    Matrix projection = Matrix::Perspective(Matrix::ConvertToRadians(45.0f), 800.0f / 600.0f, 0.1f, 400.0f);

    int num_cubes = 1000;
    std::vector<Matrix> *positions = GeneratePosition(num_cubes);
    std::vector<Quaternion> *rotations = GenerateRotationsAxis(num_cubes);

    CubeRenderer::InitializeCubes(4.0f, vao, vbo, ebo, &view_mat, &projection, world_properties->shader_id);
    CubeRenderer::AddVerticesToBuffers();

    glBindVertexArray(vao);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUseProgram(world_properties->shader_id);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //using black to clear the background
    std::cout << "Entering Main Loop" << std::endl;

    float deltaTime = 0.0f; // Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    int frame_count = 0;
    float prev_time = glfwGetTime();
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
            Matrix model = Matrix::CreateTranslationMatrix(positions->at(i));

            Quaternion q(0, 1, 1, 1);
            rotations->at(i) += q * (rotations->at(i) * 0.001);
            rotations->at(i).Normalise();
            Matrix r = Quaternion::GetMatrixTransformation(rotations->at(i)); //normalise matrix??
            Matrix model_final = Matrix::MatMul(r, model); // maybe have a matrix function that applied the matmul inside of model


            /*__m128 v_avx = _mm_setr_ps((float) rand(), rand(), rand(), rand());
            __m128 col0 = _mm_setr_ps(rand(), rand(), rand(), rand());
            __m128 col1 = _mm_setr_ps(rand(), rand(), rand(), rand());
            __m128 col2 = _mm_setr_ps(rand(), rand(), rand(), rand());
            __m128 col3 = _mm_setr_ps(rand(), rand(), rand(), rand());*/
            /*float col0[4] = {(float) rand(), (float) rand(), (float) rand(), (float) rand()};
            float col1[4] = {(float) rand(), (float) rand(), (float) rand(), (float) rand()};
            float col2[4] = {(float) rand(), (float) rand(), (float) rand(), (float) rand()};
            float col3[4] = {(float) rand(), (float) rand(), (float) rand(), (float) rand()};
            float vec[4] = {(float) rand(), (float) rand(), (float) rand(), (float) rand()};
            for(int i=0; i<4; i++) {
                Matrix::MatMulAVX4v(col0, col1, col2, col3, vec);
            }*/

            view_mat = camera.CalculateViewMat();
            CubeRenderer::ApplyUniforms(model_final);

            Matrix colour = Matrix::CreateColumnVec(1, 1, 1);
            int colour_loc = glGetUniformLocation(CubeRenderer::shader_id, "colour");
            glUniform3fv(colour_loc, 1, colour.GetValues());

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(world_properties->window);
        glfwPollEvents(); //checks if any events are triggered, and calls their respective handlers

        frame_count++;
        if(currentFrame - prev_time >= 1.0) {
            std::cout << "FPS: " << frame_count << std::endl;
            frame_count = 0;
            prev_time = currentFrame;
        }

    }

    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glfwTerminate();
    return 0;
}

std::vector<Matrix> *GeneratePosition(int num_cubes)
{
    srand((unsigned)time(NULL));
    auto *positions = new std::vector<Matrix>;
    float world_size = 100.0;
    float const scale = world_size * 2 / (float)RAND_MAX;
    auto get_coord = [scale]() -> float { return scale * (rand() - (float) RAND_MAX / 2); };
    for (int i = 0; i < num_cubes; i++)
    {
        positions->push_back(Matrix::CreateColumnVec(get_coord(), get_coord(), get_coord()));
    }
    return positions;
}

std::vector<Quaternion> *GenerateRotationsAxis(int num_cubes)
{
    srand((unsigned)time(NULL)); //NULL???
    auto *rotations = new std::vector<Quaternion>;
    for (int i = 0; i < num_cubes; i++)
    {
        rotations->push_back(Quaternion(0, (rand() % 5) - 2, (rand() % 5) - 2, (rand() % 5 - 2)));
    }
    return rotations;
}
