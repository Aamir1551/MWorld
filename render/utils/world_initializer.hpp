#ifndef MWORLD_RENDER_UTILS_WORLD_INITIALIZER_H
#define MWORLD_RENDER_UTILS_WORLD_INITIALIZER_H
#include <iostream>
#include <string>

namespace render_utils
{

    struct WorldProperties
    {
        GLFWwindow *window;
        unsigned int shader_id;
    };

    void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
    WorldProperties *WorldIntializer();

} // namespace render_utils
#endif