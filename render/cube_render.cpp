#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "helpers.cpp"
#include "./shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cube.hpp>
#include <cube_collection.hpp>

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao); // this only gives a number to the vao varialbe, doesnt acc create a buffer
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    ourShader.use();

    glm::mat4 id = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    Matrix model_mat(4, 4, glm::value_ptr(model));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f)); //translating the scene in reverse direction
    Matrix view_mat(4, 4, glm::value_ptr(view));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(0.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    Matrix proj_mat(4, 4, glm::value_ptr(projection));

    real pos[3] = {0, 0, 0};
    Matrix a(3, 1, pos);
    Cube c(a, 1.0f, ourShader.shader_id);
    c.SetModel(model_mat);
    c.SetView(view_mat);
    c.SetProject(proj_mat);
    ShaderCubeCollection s(ourShader.shader_id, vao, vbo, ebo);
    s.add_cube(&c);
    s.AddVerticesToBuffers();

    glBindVertexArray(vao);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    { // render loop -- an iteration of this main render loop is called a frame

        //s.ApplyUniforms();

        float timeValue = glfwGetTime();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(timeValue * 5), glm::vec3(0.6f, 0.8f, 0.0f));

        int shader_id = ourShader.shader_id;
        int model_loc = glGetUniformLocation(shader_id, "model"); // "model" will need to be different for every cube
        int view_loc = glGetUniformLocation(shader_id, "view");
        int proj_loc = glGetUniformLocation(shader_id, "proj");

        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(id));

        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(ourShader.shader_id);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwPollEvents(); //checks if any events are triggered, and calls their respective handlers
    }

    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glfwTerminate();
    return 0;
}