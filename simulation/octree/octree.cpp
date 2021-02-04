#include <cmath>
#include <map>
#include <set>

#include <octree.hpp>
#include <block.hpp>
#include <settings.hpp>

using namespace settings;
using namespace blocks;

Octree* Octree::AddIBlockPlus(IBlock *b) {
    this->sum_i_plus += b->position;
    if(this->is_leaf) {
        this->iblocks_at_leaf_plus.insert(b);
        this->blocks_at_leaf.insert(b);
        return this;
    } else {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        return this->children[t0 + t1 * 2 + t2 * 4]->AddIBlockPlus(b);
    }
}

Octree* Octree::AddIBlockNeg(IBlock *b) {
    this->sum_i_neg += b->position;
    if(this->is_leaf) {
        this->iblocks_at_leaf_neg.insert(b);
        this->blocks_at_leaf.insert(b);
        return this;
    } else {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        return this->children[t0 + t1 * 2 + t2 * 4]->AddIBlockNeg(b);
    }
}


Octree* Octree::AddMBlockPlus(MBlock *b) {
    this->sum_m_plus += b->position;
    if(this->is_leaf) {
        this->mblocks_at_leaf_plus.insert(b);
        this->blocks_at_leaf.insert(b);
        return this;
    } else {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        return this->children[t0 + t1 * 2 + t2 * 4]->AddMBlockPlus(b);
    }
}


Octree* Octree::AddMBlockNeg(MBlock *b) {
    this->sum_m_neg += b->position;
    if(this->is_leaf) {
        this->mblocks_at_leaf_neg.insert(b);
        this->blocks_at_leaf.insert(b);
        return this;
    } else {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        return this->children[t0 + t1 * 2 + t2 * 4]->AddMBlockNeg(b);
    }
}

Octree* Octree::AddEBlock(EBlock *b) {
    this->sum_e += b->position;
    if(this->is_leaf) {
        this->eblocks_at_leaf.insert(b);
        this->blocks_at_leaf.insert(b);
        return this;
    } else {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        return this->children[t0 + t1 * 2 + t2 * 4]->AddEBlock(b);
    }
}


Octree* Octree::AddZBlock(ZBlock *b) {
    this->sum_z += b->position;
    if(this->is_leaf) {
        this->zblocks_at_leaf.insert(b);
        this->blocks_at_leaf.insert(b);
        return this;
    } else {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        return this->children[t0 + t1 * 2 + t2 * 4]->AddZBlock(b);
    }
}

void Octree::RemoveIBlockPlus(IBlock *b) {
    this->sum_i_plus -= b->position;
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveIBlockPlus(b);
    } else {
        this->iblocks_at_leaf_plus.erase(b);
        this->blocks_at_leaf.erase(b);
    }
}

void Octree::RemoveIBlockNeg(IBlock *b) {
    this->sum_i_neg -= b->position;
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveIBlockNeg(b);
    } else {
        this->iblocks_at_leaf_neg.erase(b);
        this->blocks_at_leaf.erase(b);
    }
}

void Octree::RemoveEBlock(EBlock *b) {
    this->sum_e -= b->position;
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveEBlock(b);
    } else {
        this->eblocks_at_leaf.erase(b);
        this->blocks_at_leaf.erase(b);
    }
}

void Octree::RemoveZBlock(ZBlock *b) {
    this->sum_z -= b->position;
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveZBlock(b);
    } else {
        this->zblocks_at_leaf.erase(b);
        this->blocks_at_leaf.erase(b);
    }
}

void Octree::RemoveMBlockPlus(MBlock *b) {
    this->sum_m_plus -= b->position;
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveMBlockPlus(b);
    } else {
        this->mblocks_at_leaf_plus.erase(b);
        this->blocks_at_leaf.erase(b);
    }
}

void Octree::RemoveMBlockNeg(MBlock *b) {
    this->sum_m_neg -= b->position;
    if(!this->is_leaf) {
        auto t0 = b->position(0, 0) > avg_x;
        auto t1 = b->position(1, 0) > avg_y;
        auto t2 = b->position(2, 0) > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->RemoveMBlockNeg(b);
    } else {
        this->mblocks_at_leaf_neg.erase(b);
        this->blocks_at_leaf.erase(b);
    }
}

/*
void Octree::CalculateCOMS() {

    real iblocks_at_leaf_plus_count = this->iblocks_at_leaf_plus.size();
    real iblocks_at_leaf_neg_count = this->iblocks_at_leaf_neg.size();
    real mblocks_at_leaf_plus_count =  this->mblocks_at_leaf_plus.size();
    real mblocks_at_leaf_neg_count =  this->mblocks_at_leaf_neg.size();
    real zblocks_at_leaf_count =  this->zblocks_at_leaf.size();
    real eblocks_count =  this->eblocks_at_leaf.size();

    if(iblocks_at_leaf_plus_count > 0) {
        this->com_i_plus = Matrix::CreateColumnVec(sum_x_i_plus / iblocks_at_leaf_plus_count,
                                                   sum_y_i_plus / iblocks_at_leaf_plus_count,
                                                   sum_z_i_plus / iblocks_at_leaf_plus_count);
    }

    if(iblocks_at_leaf_neg_count > 0) {
        this->com_i_neg = Matrix::CreateColumnVec(sum_x_i_neg / iblocks_at_leaf_neg_count,
                                                  sum_y_i_neg / iblocks_at_leaf_neg_count,
                                                  sum_z_i_neg / iblocks_at_leaf_neg_count);
    }

    if(mblocks_at_leaf_plus_count > 0) {
        this->com_m_plus = Matrix::CreateColumnVec(sum_x_m_plus / mblocks_at_leaf_plus_count,
                                                   sum_y_m_plus / mblocks_at_leaf_plus_count,
                                                   sum_z_m_plus / mblocks_at_leaf_plus_count);
    }


    if(mblocks_at_leaf_neg_count > 0) {
        this->com_m_neg = Matrix::CreateColumnVec(sum_x_m_neg / mblocks_at_leaf_neg_count,
                                                  sum_y_m_neg / mblocks_at_leaf_neg_count,
                                                  sum_z_m_neg / mblocks_at_leaf_neg_count);
    }


    if(eblocks_count > 0) {
        this->com_e = Matrix::CreateColumnVec(sum_x_e / eblocks_count,
                                              sum_y_e / eblocks_count,
                                              sum_z_e / eblocks_count);
    }


    if(zblocks_at_leaf_count > 0) {
        this->com_z = Matrix::CreateColumnVec(sum_x_z / zblocks_at_leaf_count,
                                              sum_y_z / zblocks_at_leaf_count,
                                              sum_z_z / zblocks_at_leaf_count);

    }
}*/

Octree::Octree(int grid_size, real min_x, real  max_x, real min_y, real max_y, real min_z, real max_z, bool initialise) : max_x(max_x), min_x(min_x), min_y(min_y), max_y(max_y), min_z(min_z), max_z(max_z) {
    this->sum_i_plus = Matrix(3, 1, 0);
    this->sumi_neg = Matrix(3, 1, 0);
    this->sum_z = Matrix(3, 1, 0);
    this->sum_m_plus = Matrix(3, 1, 0);
    this->sum_m_neg = Matrix(3, 1, 0);
    this->sum_e = Matrix(3, 1, 0);

    this->partition_size = std::min(max_x - min_x, std::min(max_y - min_y, max_z - min_z));
    this->grid_size = grid_size;

    avg_x = (min_x + max_x) / 2;
    avg_y = (min_y + max_y) / 2;
    avg_z = (min_z + max_z) / 2;

    if (this->partition_size <= grid_size) {
        this->is_leaf = true;
    } else {
        this->is_leaf = false;
        children[0] = new Octree(grid_size, min_x, avg_x, min_y, avg_y, min_z, avg_z);
        children[1] = new Octree(grid_size, avg_x, max_x, min_y, avg_y, min_z, avg_z);
        children[2] = new Octree(grid_size, min_x, avg_x, avg_y, max_y, min_z, avg_z);
        children[3] = new Octree(grid_size, avg_x, max_x, avg_y, max_y, min_z, avg_z);
        children[4] = new Octree(grid_size, min_x, avg_x, min_y, avg_y, avg_z, max_z);
        children[5] = new Octree(grid_size, avg_x, max_x, min_y, avg_y, avg_z, max_z);
        children[6] = new Octree(grid_size, min_x, avg_x, avg_y, max_y, avg_z, max_z);
        children[7] = new Octree(grid_size, avg_x, max_x, avg_y, max_y, avg_z, max_z);
    }

    if (initialise) {
        real partition_min_x = partition_size;
        real partition_min_y = partition_size;
        real partition_min_z = partition_size;

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
                    Octree *octree_pos = GetGridAtPos(i, j, k);
                    this->grid_elements_neighbours[octree_pos] = GetGridNeighbours(i, j, k);
                    k += partition_min_z;
                }
                j += partition_min_y;
            }
            i += partition_min_x;
        }
    }
}


std::vector<Octree *> Octree::GetGridNeighbours(real x, real y, real z) {
    vector<Octree *> neighbours;

    for(int i=0; i<27; i++) {
        int a = (i % 3) - 1; // -2, -1, 0, 1, 2
        int b = (((i - (i % 3))/3) % 3) - 1;
        int c = ((i - (i%9))/9) - 1;

        real partition_min_x = partition_size;
        real partition_min_y = partition_size;
        real partition_min_z = partition_size;

        while (partition_min_x > (real) grid_size) {
            partition_min_x /= 2;
        };

        while (partition_min_y > (real) grid_size) {
            partition_min_y /= 2;
        };

        while (partition_min_z > (real) grid_size) {
            partition_min_z /= 2;
        };

        real vx = x + partition_min_x * a;
        real vy = y + partition_min_y * b;
        real vz = z + partition_min_z * c;

        if(min_x < vx && vx < this->max_x && min_y < vy && vy < max_y && min_z < vz && vz < max_z) {
            AddGridAtPosToVec(vx, vy, vz, neighbours);
        }
    }
    return neighbours;
}

void Octree::AddGridAtPosToVec(real x, real y, real z, vector<Octree *> &octree_list) {
    if(this->is_leaf) {
        octree_list.push_back(this);

    } else {
        auto t0 = x > avg_x;
        auto t1 = y > avg_y;
        auto t2 = z > avg_z;
        this->children[t0 + t1 * 2 + t2 * 4]->AddGridAtPosToVec(x, y, z, octree_list);
    }
}

Octree* Octree::GetGridAtPos(real x, real y, real z) {

    if(x > this->max_x || y > this->max_y || z > this->max_z) {
        cout << x << " " << y  << " " << z;
        cout << "error" << endl;
        exit(-1);
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

bool Octree::LeafsAreNull() {

    if(this->is_leaf) {
        return this->blocks_at_leaf.size() == 0;
    } else {
        bool cond = true;
        for(int i=0; i<this->children.size(); i++) {
            cond &= this->children.at(i)->LeafsAreNull();
            if(!cond) {
                return false;
            }
        }
    }
    return true;
}

bool Octree::BlockInCorrectTree(Octree *tree, Block *b) {

    real x = b->position(0,0);
    real y = b->position(1,0);
    real z = b->position(2,0);
    if(tree->min_x <= x && tree->max_x >= x && tree->min_y <= y && tree->max_y >= y && tree->min_z <= z && tree->max_z >= z) {
        return true;
    }
    return false;

}
