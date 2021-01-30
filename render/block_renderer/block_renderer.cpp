#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <block_renderer.hpp>
#include <cube_renderer.hpp>
#include <world_initializer.hpp>
#include <settings.hpp>
#include <camera.hpp>
#include <block.hpp>

using namespace std;

using namespace render_utils;
using namespace numerics;
using namespace settings;
using namespace blocks;



namespace render_utils {

    glm::mat4 BlockRenderer::id;
    Camera *BlockRenderer::camera;
    glm::mat4 BlockRenderer::view;
    glm::mat4 BlockRenderer::projection;

        void BlockRenderer::DrawBlocks(vector<Block *> *block_list, glm::vec3 colour) {
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
                view = BlockRenderer::camera->CalculateView();
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
        }

        void  BlockRenderer::DrawAllBlocks(vector<IBlock *> *iblocks, vector<ZBlock *> *zblocks, vector<EBlock *> *eblocks, vector<MBlock *> *mblocks) {
            // I blocks are coloured white = (1, 1, 1)
            // Z blocks are coloured pink = (1, 0, 1)
            // E blocks are coloured blue = (0, 0, 1)
            // M blocks are coloured orange = (1, 0.5, 0)

            DrawBlocks((vector<Block*> *) (iblocks), glm::vec3(1, 1, 1));
            DrawBlocks((vector<Block*> *) (zblocks), glm::vec3(1, 0, 1));
            DrawBlocks((vector<Block*> *) (eblocks), glm::vec3(0, 0, 1));
            DrawBlocks((vector<Block*> *) (mblocks), glm::vec3(1, 0.5, 0));
        }


        void BlockRenderer::InitialiseBlockRenderer(Camera *_camera, real _cube_length, unsigned int vao, unsigned int vbo , unsigned int ebo, WorldProperties *world_properties) {
            BlockRenderer::id = glm::mat4(1.0f);
            BlockRenderer::view = _camera->CalculateView();
            BlockRenderer::projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 400.0f);
            BlockRenderer::camera = _camera;

            CubeRenderer::InitializeCubes(_cube_length, vao, vbo, ebo, &view, &projection, world_properties->shader_id);
            CubeRenderer::AddVerticesToBuffers();
        };

};
