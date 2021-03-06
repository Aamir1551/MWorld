#ifndef MWORLD_RENDER_BLOCK_RENDERER_H
#define MWORLD_RENDER_BLOCK_RENDERER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cube_renderer.hpp>
#include <world_initializer.hpp>
#include <settings.hpp>
#include <matrix.hpp>
#include <camera.hpp>
#include <block.hpp>

using namespace std;

using namespace render_utils;
using namespace numerics;
using namespace settings;
using namespace blocks;


namespace render_utils {

    class BlockRenderer {

    public:

        static Matrix id;
        static Matrix view;
        static Matrix projection;

        static Camera *camera;
        static real cube_length;

        void static DrawBlocks(vector<Block *> *block_list, Matrix &colour);

        void static DrawAllBlocks(vector<IBlock *> *iblocks, vector<ZBlock *> *zblocks, vector<EBlock *> *eblocks, vector<MBlock *> *mblocks);

        void static InitialiseBlockRenderer(Camera *_camera, real _cube_length, unsigned int vao, unsigned int vbo , unsigned int ebo, WorldProperties *world_properties);

    };

}
#endif