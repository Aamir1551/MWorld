#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

    Matrix BlockRenderer::id = Matrix(4, 4, 1);
    Matrix BlockRenderer::view = Matrix(4, 4, 1);
    Matrix BlockRenderer::projection = Matrix(4, 4, 1);

    Camera *BlockRenderer::camera;

        void BlockRenderer::DrawBlocks(vector<Block *> *block_list, Matrix &colour) {
            for(auto & block_ptr : *block_list) {
                Matrix model = Matrix::CreateTranslationMatrix(block_ptr->position);

                model = Matrix::MatMul(block_ptr->GetOrientationMatrix(), model); // maybe have a matrix function that applied the matmul inside of model

                CubeRenderer::ApplyUniforms(model);

                int colour_loc = glGetUniformLocation(CubeRenderer::shader_id, "colour");
                glUniform3fv(colour_loc, 1, colour.GetValues());
                view = BlockRenderer::camera->CalculateViewMat();
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
        }

        void  BlockRenderer::DrawAllBlocks(vector<IBlock *> *iblocks, vector<ZBlock *> *zblocks, vector<EBlock *> *eblocks, vector<MBlock *> *mblocks) {
            // I blocks are coloured white = (1, 1, 1)
            // Z blocks are coloured pink = (1, 0, 1)
            // E blocks are coloured blue = (0, 0, 1)
            // M blocks are coloured orange = (1, 0.5, 0)
            static Matrix white = Matrix::CreateColumnVec(1, 1, 1);
            static Matrix pink = Matrix::CreateColumnVec(1, 0, 1);
            static Matrix blue = Matrix::CreateColumnVec(0, 0, 1);
            static Matrix orange = Matrix::CreateColumnVec(1, (float) 0.5, 0);

            DrawBlocks((vector<Block*> *) (iblocks), white);
            DrawBlocks((vector<Block*> *) (zblocks), pink);
            DrawBlocks((vector<Block*> *) (eblocks), blue);
            DrawBlocks((vector<Block*> *) (mblocks), orange);
        }


        void BlockRenderer::InitialiseBlockRenderer(Camera *_camera, real _cube_length, unsigned int vao, unsigned int vbo , unsigned int ebo, WorldProperties *world_properties) {
            BlockRenderer::id = Matrix(4, 4, 1);
            BlockRenderer::view = _camera->CalculateViewMat();
            BlockRenderer::projection = Matrix::Perspective(Matrix::ConvertToRadians(45.0f), 800.0f / 600.0f, 0.1f, 800.0f);

            BlockRenderer::camera = _camera;

            CubeRenderer::InitializeCubes(_cube_length, vao, vbo, ebo, &view, &projection, world_properties->shader_id);
            CubeRenderer::AddVerticesToBuffers();
        };

};
