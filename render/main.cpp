#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "helpers.cpp"

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
    //float verticesY[] = {0.5f, -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f}; //only first 4 elements are poitions, the rest are the colors of the 3 vertices
    float verticesY[] = {0.5f, -0.5f, -0.5f, 0.5f};
    unsigned int indices[] = {0, 1, 2, 3, 0, 2};

    glViewport(0, 0, 800, 600);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    char const *vp = "vertex";
    compileCode(vertexShader, vertexShaderSource, vp);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    char const *fp = "fragment";
    compileCode(fragmentShader, fragmentShaderSource, fp);

    unsigned int shaderProgram = glCreateProgram();
    attachProgramAndLink(shaderProgram, vertexShader, fragmentShader);

    unsigned int VAO, VBOX, VBOY, EBO, VBOC;
    glGenVertexArrays(1, &VAO); // this only gives a number to the vao varialbe, doesnt acc create a buffer
    glGenBuffers(1, &VBOX);
    glGenBuffers(1, &VBOY);
    glGenBuffers(1, &VBOC);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBOX); //makes the vbo that is specified by the id by the variable vbo, to be the one we are currently in control of
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesX), verticesX, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void *)0);

    /* each vertex attribute takes its data from memory 
    managed by a VBO and which VBO it takes its data from, is determined by the VBO currently bound to 
    GL_ARRAY_BUFFER */

    glBindBuffer(GL_ARRAY_BUFFER, VBOY); //makes the vbo that is specified by the id by the variable vbo, to be the one we are currently in control of
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesY), verticesY, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void *)0);

    /*glBindBuffer(GL_ARRAY_BUFFER, VBOC); //makes the vbo that is specified by the id by the variable vbo, to be the one we are currently in control of
    glBufferData(GL_ARRAY_BUFFER, 9, verticesY, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)4);*/

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbinding vbo
    glBindVertexArray(0);             //unbinding vao

    glUseProgram(shaderProgram);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    { // render loop -- an iteration of this main render loop is called a frame

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform3f(vertexColorLocation, 0.0f, greenValue, 0.0f);

        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwPollEvents(); //checks if any events are triggered, and calls their respective handlers
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBOX);
    glDeleteBuffers(1, &VBOY);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}