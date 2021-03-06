#include <cmath>
#include <map>

#include <force_octree.hpp>
#include <block.hpp>
#include <settings.hpp>

using namespace settings;
using namespace blocks;

// The Add block functions work similarly, by traversing the octree down, and incrementing the mass of the object at each node
ForceOctree* ForceOctree::AddIBlockPlus(IBlock *b) {
#pragma omp atomic
    this->iblocks_at_cell_plus_count +=1;
#pragma omp critical
    {
        this->sum_i_plus += b->position;
    };
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
#pragma omp atomic
    this->iblocks_at_cell_neg_count +=1;
#pragma omp critical
    {
        this->sum_i_neg += b->position;
    };
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
#pragma omp atomic
    this->mblocks_at_cell_plus_count +=1;
#pragma omp critical
    {
        this->sum_m_plus += b->position;
    };
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
#pragma omp atomic
    this->mblocks_at_cell_neg_count +=1;
#pragma omp critical
    {
        this->sum_m_neg += b->position;
    };
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
#pragma omp atomic
    this->eblocks_at_cell_count +=1;
#pragma omp critical
    {
        this->sum_e += b->position;
    };
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

#pragma omp atomic
    this->zblocks_at_cell_count +=1;
#pragma omp critical
    {
        this->sum_z += b->position;
    }
    if(this->is_leaf) {
        return this;
    } else {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        return this->children[t0 + t1 * 2 + t2 * 4]->AddZBlock(b);
    }
}

// The remove block functions work similarly, by traversing the octree down, and removing the mass of the object at each level
void ForceOctree::RemoveIBlockPlus(IBlock *b) {
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveIBlockPlus(b);
    }
#pragma omp atomic
    this->iblocks_at_cell_plus_count -=1;
#pragma omp critical
    {
        this->sum_i_plus -= b->position;
    };
}

void ForceOctree::RemoveIBlockNeg(IBlock *b) {
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveIBlockNeg(b);
    }


#pragma omp atomic
    this->iblocks_at_cell_neg_count -=1;
#pragma omp critical
    {
        this->sum_i_neg -= b->position;
    };
}

void ForceOctree::RemoveEBlock(EBlock *b) {
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveEBlock(b);
    }

#pragma omp atomic
    this->eblocks_at_cell_count -=1;
#pragma omp critical
    {
        this->sum_e -= b->position;
    };
}

void ForceOctree::RemoveZBlock(ZBlock *b) {

    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveZBlock(b);
    }
#pragma omp atomic
    this->zblocks_at_cell_count-=1;
#pragma omp critical
    {
        this->sum_z -= b->position;
    }
}

void ForceOctree::RemoveMBlockPlus(MBlock *b) {
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveMBlockPlus(b);
    }

#pragma omp atomic
    this->mblocks_at_cell_plus_count -=1;
#pragma omp critical
    {
        this->sum_m_plus -= b->position;
    };
}

void ForceOctree::RemoveMBlockNeg(MBlock *b) {
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveMBlockNeg(b);
    }

#pragma omp atomic
    this->mblocks_at_cell_neg_count -=1;
#pragma omp critical
    {
        this->sum_m_neg -= b->position;
    };
}


void ForceOctree::CalculateCOMS() {

    // Center of mass is given by sum of masses / number of blocks
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
    // Calculating COM recursively
    int num_blocks_count = zblocks_at_cell_count + eblocks_at_cell_count + iblocks_at_cell_neg_count + iblocks_at_cell_plus_count + mblocks_at_cell_neg_count + mblocks_at_cell_plus_count;
    if(num_blocks_count > 0 && !this->is_leaf) {
        for(auto & i : this->children){
            i->CalculateCOMonTree();
        }
    }
};



void ForceOctree::ApplyBarnesHutOnBlock(Block *b, real delta_time) {
    bool recurse = b->ReactBarnesHut(this, delta_time);
    if(recurse) {
        for(auto & i : this->children) {
            i->ApplyBarnesHutOnBlock(b, delta_time);
        }
    }
}

ForceOctree::ForceOctree(int grid_size, real min_x, real  max_x, real min_y, real max_y, real min_z, real max_z) : Octree(grid_size, min_x,  max_x, min_y, max_y, min_z, max_z) {
    iblocks_at_cell_plus_count= iblocks_at_cell_neg_count= mblocks_at_cell_plus_count= mblocks_at_cell_neg_count= zblocks_at_cell_count= eblocks_at_cell_count = 0;
    this->sum_i_plus = Matrix(3, 1, 0);
    this->sum_i_neg = Matrix(3, 1, 0);
    this->sum_z = Matrix(3, 1, 0);
    this->sum_m_plus = Matrix(3, 1, 0);
    this->sum_m_neg = Matrix(3, 1, 0);
    this->sum_e = Matrix(3, 1, 0);


    // Creating children recursively, until the space represented by node is less than grid_size
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
            children[i] =  new ForceOctree(grid_size, xx0, xx1, yy0, yy1, zz0, zz1);
        }
    }
}

ForceOctree::~ForceOctree() {
    if(!this->is_leaf) {
        for(auto & i : children) {
            delete i;
        }
    }
}

void ForceOctree::RemoveAllBlocks() {

    int s = iblocks_at_cell_plus_count + iblocks_at_cell_neg_count + mblocks_at_cell_plus_count + mblocks_at_cell_neg_count + zblocks_at_cell_count + eblocks_at_cell_count;
    sum_m_plus = Matrix::CreateColumnVec(0, 0, 0);
    sum_m_neg = Matrix::CreateColumnVec(0, 0, 0);
    sum_i_plus = Matrix::CreateColumnVec(0, 0, 0);
    sum_i_neg = Matrix::CreateColumnVec(0, 0, 0);
    sum_e = Matrix::CreateColumnVec(0, 0, 0);
    sum_z = Matrix::CreateColumnVec(0, 0, 0);
    iblocks_at_cell_plus_count = 0;
    iblocks_at_cell_neg_count = 0;
    mblocks_at_cell_plus_count =0;
    mblocks_at_cell_neg_count =0;
    zblocks_at_cell_count =0;
    eblocks_at_cell_count = 0;

    if(!this->is_leaf && s > 0) {
        for(int i =0; i<8; i++) {
            this->children[i]->RemoveAllBlocks();
        }
    }
}
