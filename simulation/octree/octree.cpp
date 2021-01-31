#include <cmath>
#include <map>

#include <octree.hpp>
#include <block.hpp>
#include <settings.hpp>

using namespace blocks;
using namespace settings;


void Octree::AddBlock(Block *b, unsigned int id){
    this->world_blocks[id] = b;
    if(!this->is_min) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = (b->position(1, 0) > avg_y) * 2;
        auto t2 = (b->position(2, 0) > avg_z) * 4;
        this->children[t0 + t1 + t2]->AddBlock(b, id);
    }
}

void Octree::RemoveBlock(Block *b, unsigned int id) {
    this->world_blocks.erase(id);
    if(!this->is_min) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveBlock(b, id);
    }
}

Octree::Octree(real grid_size, real min_x, real  max_x, real min_y, real max_y, real min_z, real max_z) : max_x(max_x), min_x(min_x), min_y(min_y), max_y(max_y), min_z(min_z), max_z(max_z)
{

    this->partition_size = std::min(max_x - min_x, std::min(max_y - min_y, max_z - min_z));

    avg_x = (min_x + max_x) / 2;
    avg_y = (min_y + max_y) / 2;
    avg_z = (min_z + max_z) / 2;

    if(this->partition_size < grid_size) {
        this->is_min = true;
    } else {
        children[0] = new Octree(grid_size, min_x, avg_x, min_y, avg_y, min_z, avg_z);
        children[1] = new Octree(grid_size, avg_x, max_x, min_y, avg_y, min_z, avg_z);
        children[2] = new Octree(grid_size, min_x, avg_x, avg_y, max_y, min_z, avg_z);
        children[3] = new Octree(grid_size, avg_x, max_x, avg_y, max_y, min_z, avg_z);
        children[4] = new Octree(grid_size, min_x, avg_x, min_y, avg_y, avg_z, max_z);
        children[5] = new Octree(grid_size, avg_x, max_x, min_y, avg_y, avg_z, max_z);
        children[6] = new Octree(grid_size, min_x, avg_x, avg_y, max_y, avg_z, max_z);
        children[7] = new Octree(grid_size, avg_x, max_x, avg_y, max_y, avg_z, max_z);
    }

}

std::vector<Block *> Octree::GetBlockNeighbours(Block *b) {





    return std::vector<Block *>();
}

Octree *Octree::GetGridAtPos(real x, real y, real z) {
    if(!this->is_min) {
        auto t0 = x > avg_x;
        auto t1 = y > avg_y;
        auto t2 = z > avg_z;
        return this->children[t0 + t1 * 2 + t2 * 4]->GetGridAtPos(x, y, z);
    }
    if(x > this->)
    return this;
}
