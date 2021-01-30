#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.hpp>
#include <world_initializer.hpp>

namespace render_utils
{

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

        //char result[MAX_PATH]; --first use #include <windows.h>

        //std::cout << std::string(result, GetModuleFileName(NULL, result, MAX_PATH)) << std::endl; --first use #include<windows.h>
        Shader shader("../../../render/glsl_shaders/vert.glsl", "../../../render/glsl_shaders/frag.glsl"); //with respect to root build

        shader.use();
        std::cout << "Shaders successfully initialised" << std::endl;
        WorldProperties *world_properties = new WorldProperties({window, shader.shader_id});
        return world_properties;
    }
} // namespace render_utils