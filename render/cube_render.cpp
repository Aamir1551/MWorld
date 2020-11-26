#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "./shader.hpp"

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
    glm::mat4 model = glm::rotate(id, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 view = glm::translate(id, glm::vec3(0.0f, 0.0f, 0.0f)); //translating the scene in reverse direction
    glm::mat4 projection = glm::perspective(glm::radians(0.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    Cube c(0, 0, 0, 1.0f, ourShader.shader_id);
    c.model = &model;
    c.view = &id;
    c.project = &id;

    ShaderCubeCollection s(ourShader.shader_id, vao, vbo, ebo);
    s.add_cube(&c);
    s.AddVerticesToBuffers();

    glBindVertexArray(vao);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    { // render loop -- an iteration of this main render loop is called a frame

        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float timeValue = glfwGetTime();
        model = glm::rotate(id, glm::radians(timeValue * 5), glm::vec3(0.6f, 0.8f, 0.0f));
        s.ApplyUniforms();

        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(ourShader.shader_id);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

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