#ifndef MWORLD_RENDER_BLOCK_RENDERER_H
#define MWORLD_RENDER_BLOCK_RENDERER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cube_renderer.hpp>
#include <world_initializer.hpp>
#include <settings.hpp>
#include <matrix.hpp>
#include <camera.hpp>
#include <block.hpp>
#include <world_handler.cpp>

using namespace std;

using namespace render_utils;
using namespace numerics;
using namespace settings;
using namespace blocks;


namespace render_utils {

    class BlockRenderer {

    public:

        static glm::mat4 id;
        static Camera camera;
        static real cube_length;
        static glm::mat4 view;
        static glm::mat4 projection;

        void static DrawBlocks(vector<Block *> *block_list,glm::vec3 colour, glm::mat4& id, Camera &camera, glm::mat4 &view) {
            for(auto & block_ptr : *block_list) {
                glm::mat4 rotation_mat;
                memcpy(glm::value_ptr(rotation_mat), block_ptr->GetOrientationMatrix().GetValues(), 16 * sizeof(real));

                glm::vec3 translation_mat;
                memcpy(glm::value_ptr(translation_mat), block_ptr->position.GetValues(), 3 * sizeof(real));

                glm::mat4 model = glm::translate(id, translation_mat);
                model = model * rotation_mat;
                CubeRenderer::ApplyUniforms(model);

                int colour_loc = glGetUniformLocation(CubeRenderer::shader_id, "colour");
                glUniform3fv(colour_loc, 1, glm::value_ptr(colour));
                view = camera.CalculateView();
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
        }

        void static DrawAllBlocks(WorldHandler &world) {
            // I blocks are coloured white = (1, 1, 1)
            // Z blocks are coloured pink = (1, 0, 1)
            // E blocks are coloured blue = (0, 0, 1)
            // M blocks are coloured orange = (1, 0.5, 0)

            DrawBlocks((vector<Block*> *) &(world.iblocks), glm::vec3(1, 1, 1), BlockRenderer::id, BlockRenderer::camera, BlockRenderer::view);
            DrawBlocks((vector<Block*> *) &(world.zblocks), glm::vec3(1, 0, 1), BlockRenderer::id, BlockRenderer::camera, BlockRenderer::view);
            DrawBlocks((vector<Block*> *) &(world.eblocks), glm::vec3(0, 0, 1), BlockRenderer::id, BlockRenderer::camera, BlockRenderer::view);
            DrawBlocks((vector<Block*> *) &(world.mblocks), glm::vec3(1, 0.5, 0), BlockRenderer::id, BlockRenderer::camera, BlockRenderer::view);
        }


        void static InitialiseBlockRenderer(Camera &_camera, real _cube_length, unsigned int vao, unsigned int vbo , unsigned int ebo, WorldProperties *world_properties) {
            BlockRenderer::view = _camera.CalculateView();
            BlockRenderer::projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 400.0f);
            BlockRenderer::cube_length = _cube_length;
            BlockRenderer::camera = _camera;

            CubeRenderer::InitializeCubes(_cube_length, vao, vbo, ebo, &view, &projection, world_properties->shader_id);
            CubeRenderer::AddVerticesToBuffers();
        };

    };

}
#endif