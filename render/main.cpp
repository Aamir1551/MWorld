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
//#include <cube.hpp>

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

    float verticesX[] = {0.5f, 0.5f, -0.5f, -0.5f};
    float verticesY[] = {0.5f, -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f}; //only first 4 elements are poitions, the rest are the colors of the 3 vertices
    //float verticesY[] = {0.5f, -0.5f, -0.5f, 0.5f};
    unsigned int indices[] = {0, 1, 2, 3, 0, 2};

    glViewport(0, 0, 800, 600);

    Shader ourShader("../shaders/vert.glsl", "../shaders/frag.glsl");

    unsigned int VAO, VBOX, VBOY, EBO, VBOC;
    glGenVertexArrays(1, &VAO); // this only gives a number to the vao varialbe, doesnt acc create a buffer
    glGenBuffers(1, &VBOX);
    glGenBuffers(1, &VBOY);
    glGenBuffers(1, &VBOC);
    glGenBuffers(1, &EBO);

    /*glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBOX); //makes the vbo that is specified by the id by the variable vbo, to be the one we are currently in control of
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesX), verticesX, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void *)0);

     each vertex attribute takes its data from memory 
    managed by a VBO and which VBO it takes its data from, is determined by the VBO currently bound to 
    GL_ARRAY_BUFFER */

    /*glBindBuffer(GL_ARRAY_BUFFER, VBOY); //makes the vbo that is specified by the id by the variable vbo, to be the one we are currently in control of
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), verticesY, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOC); //makes the vbo that is specified by the id by the variable vbo, to be the one we are currently in control of
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesY), verticesY, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)4);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbinding vbo */
    //glBindVertexArray(0);             //unbinding vao

    ourShader.use();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    Matrix model_mat(4, 4, glm::value_ptr(model));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); //translating the scene in reverse direction
    Matrix view_mat(4, 4, glm::value_ptr(view));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    Matrix proj_mat(4, 4, glm::value_ptr(projection));

    real pos[3] = {0, 0, 0};
    Matrix a(3, 1, pos);
    Cube c(a, 2, ourShader.shader_id);
    c.SetModel(model_mat);
    c.SetView(view_mat);
    c.SetProject(proj_mat);
    ShaderCubeCollection s(ourShader.shader_id);
    s.add_cube(&c);

    while (!glfwWindowShouldClose(window))
    { // render loop -- an iteration of this main render loop is called a frame

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        ourShader.set_float3("ourColor", 0.0f, greenValue, 0.0f);

        s.ApplyUniforms();

        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        /*int model_loc = glGetUniformLocation(ourShader.shader_id, "model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

        int view_loc = glGetUniformLocation(ourShader.shader_id, "view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));

        int proj_loc = glGetUniformLocation(ourShader.shader_id, "proj");
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection)); */

        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwPollEvents(); //checks if any events are triggered, and calls their respective handlers
    }

    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBOX);
    glDeleteBuffers(1, &VBOY);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}