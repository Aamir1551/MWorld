#ifndef MWORLD_RENDER_CUBE_CUBE_H
#define MWORLD_RENDER_CUBE_CUBE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <math.h>
#include <vector>

namespace render_utils
{
    class CubeRenderer
    {

    public:
        static float cube_length;
        std::vector<glm::mat4 *> models;
        static glm::mat4 *view;
        static glm::mat4 *project;
        static int vao;
        static int vbo;
        static int ebo;
        static float vertices[24];
        static unsigned int shader_id;

        static unsigned int indices[36];

        CubeRenderer(){};

        void static InitializeCubes(float cube_length, unsigned int vao, unsigned int vbo, unsigned int ebo, glm::mat4 *view, glm::mat4 *project, unsigned int shader_id)
        {
            CubeRenderer::shader_id = shader_id;
            float diff = cube_length / 2;
            for (unsigned int i = 0; i < 8; i++)
            {
                CubeRenderer::vertices[i * 3] = (i % 2 == 0 ? -diff : diff);
                CubeRenderer::vertices[i * 3 + 1] = (((int)std::floor(i / 2)) % 2 == 0 ? -diff : diff);
                CubeRenderer::vertices[i * 3 + 2] = (((int)std::floor(i / 4)) % 2 == 0 ? -diff : diff);
            }

            CubeRenderer::vao = vao;
            CubeRenderer::vbo = vbo;
            CubeRenderer::ebo = ebo;

            CubeRenderer::view = view;
            CubeRenderer::project = project;
        }

        void ApplyUniforms(int model_id);

        void static AddVerticesToBuffers()
        {
            glBindVertexArray(CubeRenderer::vao);

            glBindBuffer(GL_ARRAY_BUFFER, CubeRenderer::vbo);
            glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(float), CubeRenderer::vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
            glBindBuffer(GL_ARRAY_BUFFER, 0); // unbinding vbo

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CubeRenderer::ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(float), CubeRenderer::indices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
        }
    };
} // namespace render_utils
#endif