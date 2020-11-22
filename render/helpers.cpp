#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in float xPos; \n"
                                 "layout (location = 1) in float yPos;\n"
                                 "//layout (location = 2) in vec3 color;\n"
                                 "//out vec3 cc;\n"
                                 "void main()\n"
                                 "{\n"
                                 " gl_Position = vec4(xPos, yPos, 0, 1);\n"
                                 " //cc = color;\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColors; //fragment shader requires a vec4 output\n"
                                   "//in vec3 cc;\n"
                                   "uniform vec3 ourColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColors = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "    //FragColors = vec4(ourColor*cc, 1);\n"
                                   "    FragColors = vec4(ourColor, 1);\n"
                                   "    //FragColors = vec4(cc, 1);\n"
                                   "}\n\0";

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

void compileCode(int shaderID, char const *sourcecode, char const *shaderName)
{

    glShaderSource(shaderID, 1, &sourcecode, NULL);
    glCompileShader(shaderID);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << shaderName << "::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
}

void attachProgramAndLink(unsigned int shaderProgram, unsigned int vertexShader, unsigned int fragmentShader)
{
    int success;
    char infoLog[512];
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
}
