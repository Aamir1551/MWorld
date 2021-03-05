#ifndef MWORLD_RENDER_UTILS_WORLD_INITIALIZER_H
#define MWORLD_RENDER_UTILS_WORLD_INITIALIZER_H
#include <iostream>
#include <string>

//#include <glad/glad.h>
//#include <GLFW/glfw3.h>

//#include <shader.hpp>

namespace render_utils
{

    struct WorldProperties
    {
        GLFWwindow *window;
        unsigned int shader_id;
    };

    void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    WorldProperties *world_intializer();

} // namespace render_utils
#endif