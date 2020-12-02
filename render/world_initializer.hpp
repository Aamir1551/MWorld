#ifndef MWORLD_RENDER_WORLD_INITIALIZER_H
#define MWORLD_RENDER_WORLD_INITIALIZER_H
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.hpp>

struct WorldProperties
{
    GLFWwindow *window;
    unsigned int shader_id;
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
};

WorldProperties *world_intializer()
{
    std::cout << "Rendering started" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    std::cout << "GLFW initialised" << std::endl;

    GLFWwindow *window = glfwCreateWindow(800, 600, "Cube Renderer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create a GLFW window" << std::endl;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    std::cout << "GLFW successfully intialised" << std::endl;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    std::cout << "GLAD successfully intialised" << std::endl;

    glViewport(0, 0, 800, 600);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Shader shader("  ../shaders/vert.glsl", "../shaders/frag.glsl"); //with respect to current
    Shader shader("../../render/shaders/vert.glsl", "../../render/shaders/frag.glsl"); //with respect to root build

    shader.use();
    std::cout << "Shaders successfully initialised" << std::endl;
    WorldProperties *world_properties = new WorldProperties({window, shader.shader_id});
    return world_properties;
}
#endif