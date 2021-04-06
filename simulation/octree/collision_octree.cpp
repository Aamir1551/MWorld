#include <cmath>
#include <map>

#include <collision_octree.hpp>
#include <block.hpp>
#include <settings.hpp>
#include <octree.hpp>

using namespace settings;
using namespace blocks;

CollisionOctree* CollisionOctree::AddBlock(Block *b) {
    if(this->is_leaf) {
#pragma omp critical
        {
            this->blocks_at_leaf.insert(b);
        }
        return this;
    } else {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        return this->children[t0 + t1 * 2 + t2 * 4]->AddBlock(b);
    }
}

void CollisionOctree::RemoveBlock(Block *b) {
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveBlock(b);
    } else {
#pragma omp critical
        {
            this->blocks_at_leaf.erase(b);
        }
    }
}

CollisionOctree::CollisionOctree(int grid_size, real min_x, real  max_x, real min_y, real max_y, real min_z, real max_z, bool initialise) : Octree(grid_size, min_x,  max_x, min_y, max_y, min_z, max_z) {

    if (this->cell_partition_size <= grid_size) {
        this->is_leaf = true;
    } else {
        this->is_leaf = false;

        for(int i=0; i<8; i++) {
            auto xx0 = (i % 2) ? avg_x : min_x;
            auto xx1 = (i % 2) ? max_x: avg_x;

            auto yy0 = ((i/2) % 2) ? avg_y : min_y;
            auto yy1 = ((i/2) % 2) ? max_y : avg_y;

            auto zz0 = ((i/4) % 2) ? avg_z : min_z;
            auto zz1 = ((i/4) % 2) ? max_z : avg_z;
            children[i] = new CollisionOctree(grid_size, xx0, xx1, yy0, yy1, zz0, zz1);
        }
    }

    if (initialise) {
        real partition_min_x = cell_partition_size;
        real partition_min_y = cell_partition_size;
        real partition_min_z = cell_partition_size;

        while (partition_min_x > (real) grid_size) {
            partition_min_x /= 2;
        };

        while (partition_min_y > (real) grid_size) {
            partition_min_y /= 2;
        };

        while (partition_min_z > (real) grid_size) {
            partition_min_z /= 2;
        };

        real i=min_x + partition_min_x / 2;
        while (i < max_x) {
            real j = min_y + partition_min_y / 2;
            while (j < max_y) {
                real k = min_z + partition_min_z / 2;
                while (k < max_z) {
                    CollisionOctree *octree_pos = GetGridAtPos(i, j, k);

                    this->grid_elements_neighbours[octree_pos] = GenerateNeigbours(i, j, k, partition_min_x, partition_min_y, partition_min_z);
                    k += partition_min_z;
                }
                j += partition_min_y;
            }
            i += partition_min_x;
        }
    }
}

std::vector<CollisionOctree *> CollisionOctree::GenerateNeigbours(real x, real y, real z, real partition_min_x, real partition_min_y, real partition_min_z) {
    vector<CollisionOctree *> neighbours;

    for(int i=0; i<27; i++) {
        int a = (i % 3) - 1; // -2, -1, 0, 1, 2
        int b = (((i - (i % 3))/3) % 3) - 1;
        int c = ((i - (i%9))/9) - 1;

        /*int c = (i % 3) - 1;
        int c1 = (i - c) / 3;
        int b = c1 % 2;
        int b1 = (c1 - b) / 2;
        int a = b1 % 2;*/

        /*real partition_min_x = cell_partition_size;
        real partition_min_y = cell_partition_size;
        real partition_min_z = cell_partition_size;

        while (partition_min_x > (real) grid_size) {
            partition_min_x /= 2;
        };

        while (partition_min_y > (real) grid_size) {
            partition_min_y /= 2;
        };

        while (partition_min_z > (real) grid_size) {
            partition_min_z /= 2;
        };*/

        real vx = x + partition_min_x * a;
        real vy = y + partition_min_y * b;
        real vz = z + partition_min_z * c;

        if(min_x < vx && vx < this->max_x && min_y < vy && vy < max_y && min_z < vz && vz < max_z) {
            AddGridAtPosToVec(vx, vy, vz, neighbours);
        }
    }
    return neighbours;
}

void CollisionOctree::AddGridAtPosToVec(real x, real y, real z, vector<CollisionOctree *> &octree_list) {
    if(this->is_leaf) {
        octree_list.push_back(this);

    } else {
        auto t0 = x > avg_x;
        auto t1 = y > avg_y;
        auto t2 = z > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->AddGridAtPosToVec(x, y, z, octree_list);
    }
}

CollisionOctree* CollisionOctree::GetGridAtPos(real x, real y, real z) {

    if( x <= this->min_x || x >= this->max_x || y >= this->max_y || y <= this->min_y || z >= this->max_z || z <= this->min_z) {
        throw std::invalid_argument("Coordinates need to be within the quadtree boundary for function GetGridAtPos");
    }

    if(this->is_leaf) {
        return this;
    } else {
        auto t0 = x > avg_x;
        auto t1 = y > avg_y;
        auto t2 = z > avg_z;
        return this->children[t0 + t1 * 2 + t2 * 4]->GetGridAtPos(x, y, z);
    }
}

bool CollisionOctree::LeavesAreNull() {

    if(this->is_leaf) {
        return this->blocks_at_leaf.empty();
    } else {
        bool cond = true;
        for(auto & i : this->children) {
            cond &= i->LeavesAreNull();
            if(!cond) {
                return false;
            }
        }
    }
    return true;
}

CollisionOctree::~CollisionOctree() {
    if(!this->is_leaf) {
        for(auto & i : children) {
            delete i;
        }
    }
}
