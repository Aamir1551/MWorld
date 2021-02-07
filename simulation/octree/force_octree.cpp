#include <cmath>
#include <map>

#include <force_octree.hpp>
#include <block.hpp>
#include <settings.hpp>

using namespace settings;
using namespace blocks;

ForceOctree* ForceOctree::AddIBlockPlus(IBlock *b) {
    this->sum_i_plus += b->position;
    this->iblocks_at_cell_plus_count +=1;
    if(this->is_leaf) {
        return this;
    } else {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        return this->children[t0 + t1 * 2 + t2 * 4]->AddIBlockPlus(b);
    }
}

ForceOctree* ForceOctree::AddIBlockNeg(IBlock *b) {
    this->sum_i_neg += b->position;
    this->iblocks_at_cell_neg_count +=1;
    if(this->is_leaf) {
        return this;
    } else {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        return this->children[t0 + t1 * 2 + t2 * 4]->AddIBlockNeg(b);
    }
}


ForceOctree* ForceOctree::AddMBlockPlus(MBlock *b) {
    this->sum_m_plus += b->position;
    this->mblocks_at_cell_plus_count +=1;
    if(this->is_leaf) {
        return this;
    } else {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        return this->children[t0 + t1 * 2 + t2 * 4]->AddMBlockPlus(b);
    }
}

ForceOctree* ForceOctree::AddMBlockNeg(MBlock *b) {
    this->sum_m_neg += b->position;
    this->mblocks_at_cell_neg_count +=1;
    if(this->is_leaf) {
        return this;
    } else {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        return this->children[t0 + t1 * 2 + t2 * 4]->AddMBlockNeg(b);
    }
}

ForceOctree* ForceOctree::AddEBlock(EBlock *b) {
    this->sum_e += b->position;
    this->eblocks_at_cell_count +=1;
    if(this->is_leaf) {
        return this;
    } else {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        return this->children[t0 + t1 * 2 + t2 * 4]->AddEBlock(b);
    }
}


ForceOctree* ForceOctree::AddZBlock(ZBlock *b) {
    this->sum_z += b->position;
    this->zblocks_at_cell_count +=1;
    if(this->is_leaf) {
        return this;
    } else {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        return this->children[t0 + t1 * 2 + t2 * 4]->AddZBlock(b);
    }
}

void ForceOctree::RemoveIBlockPlus(IBlock *b) {
    this->sum_i_plus -= b->position;
    this->iblocks_at_cell_plus_count -=1;
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveIBlockPlus(b);
    }
}

void ForceOctree::RemoveIBlockNeg(IBlock *b) {
    this->sum_i_neg -= b->position;
    this->iblocks_at_cell_neg_count -=1;
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveIBlockNeg(b);
    }
}

void ForceOctree::RemoveEBlock(EBlock *b) {
    this->sum_e -= b->position;
    this->eblocks_at_cell_count -=1;
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveEBlock(b);
    }
}

void ForceOctree::RemoveZBlock(ZBlock *b) {
    this->sum_z -= b->position;
    this->zblocks_at_cell_count-=1;
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveZBlock(b);
    }
}

void ForceOctree::RemoveMBlockPlus(MBlock *b) {
    this->sum_m_plus -= b->position;
    this->mblocks_at_cell_plus_count -=1;
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveMBlockPlus(b);
    }
}

void ForceOctree::RemoveMBlockNeg(MBlock *b) {
    this->sum_m_neg -= b->position;
    this->mblocks_at_cell_neg_count -=1;
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveMBlockNeg(b);
    }
}


void ForceOctree::CalculateCOMS() {

    if(iblocks_at_cell_plus_count > 0) {
        this->com_i_plus = sum_i_plus / iblocks_at_cell_plus_count;
    }

    if(iblocks_at_cell_neg_count > 0) {
        this->com_i_neg = sum_i_neg / iblocks_at_cell_neg_count;
    }

    if(mblocks_at_cell_plus_count > 0) {
        this->com_m_plus = sum_m_plus / mblocks_at_cell_plus_count;
    }

    if(mblocks_at_cell_neg_count > 0) {
        this->com_m_neg = sum_m_neg / mblocks_at_cell_neg_count;
    }

    if(eblocks_at_cell_count > 0) {
        this->com_e = sum_e / eblocks_at_cell_count;
    }

    if(zblocks_at_cell_count > 0) {
        this->com_z = sum_z / zblocks_at_cell_count;
    }
}

void ForceOctree::CalculateCOMonTree() {
    CalculateCOMS();
    int num_blocks_count = zblocks_at_cell_count + eblocks_at_cell_count + iblocks_at_cell_neg_count + iblocks_at_cell_plus_count + mblocks_at_cell_neg_count + mblocks_at_cell_plus_count;
    if(num_blocks_count > 0 && !this->is_leaf) {
        for(int i=0; i<8; i++){
            this->children[i]->CalculateCOMonTree();
        }
    }
};



void ForceOctree::ApplyBarnesHutOnBlock(Block *b, real delta_time) {
    //this->count += 1;
    bool recurse = b->React(this, delta_time);
    if(recurse) {
        for(int i=0; i<8; i++) {
            this->children[i]->ApplyBarnesHutOnBlock(b, delta_time);
        }
    }
}

ForceOctree::ForceOctree(int grid_size, real min_x, real  max_x, real min_y, real max_y, real min_z, real max_z, bool initialise) : max_x(max_x), min_x(min_x), min_y(min_y), max_y(max_y), min_z(min_z), max_z(max_z) {
    iblocks_at_cell_plus_count= iblocks_at_cell_neg_count= mblocks_at_cell_plus_count= mblocks_at_cell_neg_count= zblocks_at_cell_count= eblocks_at_cell_count = 0;
    this->sum_i_plus = Matrix(3, 1, 0);
    this->sum_i_neg = Matrix(3, 1, 0);
    this->sum_z = Matrix(3, 1, 0);
    this->sum_m_plus = Matrix(3, 1, 0);
    this->sum_m_neg = Matrix(3, 1, 0);
    this->sum_e = Matrix(3, 1, 0);

    this->cell_partition_size = std::min(max_x - min_x, std::min(max_y - min_y, max_z - min_z));
    this->grid_size = grid_size;

    avg_x = (min_x + max_x) / 2;
    avg_y = (min_y + max_y) / 2;
    avg_z = (min_z + max_z) / 2;

    if (this->cell_partition_size <= grid_size) {
        this->is_leaf = true;
    } else {
        this->is_leaf = false;
        children[0] = new ForceOctree(grid_size, min_x, avg_x, min_y, avg_y, min_z, avg_z);
        children[1] = new ForceOctree(grid_size, avg_x, max_x, min_y, avg_y, min_z, avg_z);
        children[2] = new ForceOctree(grid_size, min_x, avg_x, avg_y, max_y, min_z, avg_z);
        children[3] = new ForceOctree(grid_size, avg_x, max_x, avg_y, max_y, min_z, avg_z);
        children[4] = new ForceOctree(grid_size, min_x, avg_x, min_y, avg_y, avg_z, max_z);
        children[5] = new ForceOctree(grid_size, avg_x, max_x, min_y, avg_y, avg_z, max_z);
        children[6] = new ForceOctree(grid_size, min_x, avg_x, avg_y, max_y, avg_z, max_z);
        children[7] = new ForceOctree(grid_size, avg_x, max_x, avg_y, max_y, avg_z, max_z);
    }
}

bool ForceOctree::BlockInCorrectTree(ForceOctree *tree, Block *b) {

    real x = b->position(0,0);
    real y = b->position(1,0);
    real z = b->position(2,0);
    if(tree->min_x <= x && tree->max_x >= x && tree->min_y <= y && tree->max_y >= y && tree->min_z <= z && tree->max_z >= z) {
        return true;
    }
    return false;

}

int ForceOctree::count = 0;