#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "./shader.hpp"
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cube.hpp>
#include <cube_collection.hpp>

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Cube Renderer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create a GLFW window" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    Shader ourShader("../shaders/vert.glsl", "../shaders/frag.glsl");
    ourShader.use();

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao); // this only gives a number to the vao varialbe, doesnt acc create a buffer
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glm::mat4 id = glm::mat4(1.0f);
    glm::mat4 model_cube1 = glm::rotate(id, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 model_cube2 = glm::rotate(id, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 view = glm::translate(id, glm::vec3(0.0f, 0.0f, -4.0f)); //translating the scene in reverse direction
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    std::vector<glm::vec3 *> models;

    auto a = glm::vec3(0.0f, 0.0f, 0.0f);
    auto b = glm::vec3(2.0f, 5.0f, -15.0f);

    //models.push_back(&model_cube1);
    //models.push_back(&model_cube2);
    models.push_back(&a);
    models.push_back(&b);

    Cube c1(1.0f, ourShader.shader_id, &model_cube1);
    Cube c2(1.0f, ourShader.shader_id, &model_cube2);
    c1.view = &view;
    c1.project = &projection;

    c2.view = &view;
    c2.project = &projection;

    ShaderCubeCollection s(ourShader.shader_id, vao, vbo, ebo);
    s.add_cube(&c1);
    //s.add_cube(&c2);
    s.AddVerticesToBuffers();

    glBindVertexArray(vao);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUseProgram(ourShader.shader_id);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    while (!glfwWindowShouldClose(window))
    { // render loop -- an iteration of this main render loop is called a frame
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < 2; i++)
        {

            //model_cube1 = glm::rotate(id, glm::radians((float)glfwGetTime() * 20), glm::vec3(0.2f, 0.8f, 0.3f));
            model_cube1 = glm::translate(id, *models.at(i));
            model_cube1 = glm::rotate(model_cube1, glm::radians((float)glfwGetTime() * 20), glm::vec3(0.2f, 0.8f, 0.3f));
            s.ApplyUniforms();
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents(); //checks if any events are triggered, and calls their respective handlers
    }

    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}