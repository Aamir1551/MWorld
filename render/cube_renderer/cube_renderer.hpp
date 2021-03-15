#ifndef MWORLD_RENDER_CUBE_RENDER_H
#define MWORLD_RENDER_CUBE_RENDER_H

#include <math.h>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <type_traits>

#include <settings.hpp>
#include <matrix.hpp>
#include <quaternion.hpp>

using namespace numerics;

namespace render_utils
{
    class CubeRenderer
    {

    public:
        static glm::mat4 *view;
        static glm::mat4 *project;
        static unsigned int vao;
        static unsigned int vbo;
        static unsigned int ebo;
        static settings::real vertices[24];
        static unsigned int shader_id;

        static unsigned int indices[36];

        CubeRenderer()= default;;

        void static InitializeCubes(settings::real _cube_length, unsigned int _vao, unsigned int _vbo, unsigned int _ebo, glm::mat4 *_view, glm::mat4 *_project, unsigned int _shader_id)
        {
            CubeRenderer::shader_id = _shader_id;
            settings::real diff = _cube_length / 2;
            for (unsigned int i = 0; i < 8; i++)
            {
                CubeRenderer::vertices[i * 3] = (i % 2 == 0 ? -diff : diff);
                CubeRenderer::vertices[i * 3 + 1] = (((int)std::floor(i / 2)) % 2 == 0 ? -diff : diff);
                CubeRenderer::vertices[i * 3 + 2] = (((int)std::floor(i / 4)) % 2 == 0 ? -diff : diff);
            }

            CubeRenderer::vao = _vao;
            CubeRenderer::vbo = _vbo;
            CubeRenderer::ebo = _ebo;

            CubeRenderer::view = _view;
            CubeRenderer::project = _project;
        }

        void static ApplyUniforms(Matrix &model_mat);
        void static ApplyUniforms(glm::mat4 &model_mat);

        void static AddVerticesToBuffers()
        {
            glBindVertexArray(CubeRenderer::vao);

            glBindBuffer(GL_ARRAY_BUFFER, CubeRenderer::vbo);
            glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(settings::real), CubeRenderer::vertices, GL_STATIC_DRAW);

            //make statement for all types
            if (std::is_same<settings::real, double>::value)
            {
                glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(settings::real), (void *)0);
            }
            else
            {
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(settings::real), (void *)0);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0); // unbinding vbo
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CubeRenderer::ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(settings::real), CubeRenderer::indices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
        }
    };
} // namespace render_utils
#endif