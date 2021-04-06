#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include <set>
#include <omp.h>
#include <cmath>
#include <iostream>

#include <matrix.hpp>

#include <cube.hpp>
#include <block.hpp>
#include <i_block.hpp>
#include <m_block.hpp>
#include <e_block.hpp>
#include <z_block.hpp>

#include <world_handler.hpp>

using namespace blocks;

std::vector<Matrix> *WorldHandler::GeneratePositions(int num_cubes, real min_coord_x, real max_coord_x,
                                                     real min_coord_y, real max_coord_y,
                                                     real min_coord_z, real max_coord_z)
{
    auto *positions = new std::vector<Matrix>;

    int world_size_x = (int) (max_coord_x - min_coord_x);
    auto get_coord_x = [world_size_x, min_coord_x]() -> real { return (rand()%( world_size_x + 1) + min_coord_x); };

    int world_size_y = (int) (max_coord_y - min_coord_y);
    auto get_coord_y = [world_size_y, min_coord_y]() -> real { return (rand()%( world_size_y + 1) + min_coord_y); };

    int world_size_z = (int) (max_coord_z - min_coord_z);
    auto get_coord_z = [world_size_z, min_coord_z]() -> real { return (rand()%( world_size_z + 1) + min_coord_z); };

    for (int i = 0; i < num_cubes; i++)
    {
        real values[] = {get_coord_x(), get_coord_y(), get_coord_z()};
        positions->push_back(Matrix(3, 1, values));
    }
    return positions;
}

std::vector<Matrix> *WorldHandler::GenerateLinearMomentums(int num_cubes)
{
    auto *linear_momentums = new std::vector<Matrix>;
    auto get_momentums = []() -> real { return ((rand() % 3) + -1) * 0.2;};
    for (int i = 0; i < num_cubes; i++)
    {
        real values[] = { get_momentums() , get_momentums() , get_momentums()};
        //real values[] = {0, 0, 0}; // Used for testing purposes
        linear_momentums->push_back(Matrix(3, 1, values));
    }
    return linear_momentums;
}

WorldHandler::WorldHandler(int num_i_blocks_plus, int num_i_blocks_neg, int num_z_blocks, int num_m_blocks, int num_e_blocks_1, int num_e_blocks_1_2,
                           real min_coord_x, real max_coord_x,
                           real min_coord_y, real max_coord_y,
                           real min_coord_z, real max_coord_z,
                           real cube_length) {
    srand((unsigned)time(0));
    //srand(0); // useful for testing purposes, to generate the same random numbers

    //cout << "Quad trees are being initialised" << endl;
    this->collision_tree = new CollisionOctree((int)cube_length * 3, min_coord_x, max_coord_x, min_coord_y, max_coord_y, min_coord_z, max_coord_z, true);
    this->forces_tree = new ForceOctree((int) cube_length * 5, min_coord_x, max_coord_x, min_coord_y, max_coord_y, min_coord_z, max_coord_z);
    //cout << "Quad trees are initialised" << endl;
    this->cube_length = cube_length;

    this->min_coord_x = min_coord_x;
    this->min_coord_y = min_coord_y;
    this->min_coord_z = min_coord_z;

    this->max_coord_x = max_coord_x;
    this->max_coord_y = max_coord_y;
    this->max_coord_z = max_coord_z;

    AddBlock(IBlockType, num_i_blocks_plus, true);
    AddBlock(IBlockType, num_i_blocks_neg, false);
    AddBlock(MBlockType, num_m_blocks, true);
    AddBlock(EBlockType, num_e_blocks_1, true);
    AddBlock(EBlockType, num_e_blocks_1_2, false);
    AddBlock(ZBlockType, num_z_blocks, true);
/*
#if defined(OPENMP)
    for(unsigned int i=0; i<this->blocks.size(); i++) {
       this->collision_locks.emplace_back(omp_lock_t());
       omp_init_lock(&this->collision_locks.at(i));
    }
#endif
 */
}

void WorldHandler::GetProperties(int num_blocks, std::vector<Matrix> *&positions, std::vector<Matrix> * &linear_momentums, real min_coord_x, real max_coord_x, real min_coord_y, real max_coord_y, real min_coord_z, real max_coord_z) {
    positions = GeneratePositions(num_blocks, min_coord_x, max_coord_x,
                                  min_coord_y, max_coord_y,
                                  min_coord_z, max_coord_z);
    linear_momentums = GenerateLinearMomentums(num_blocks);
}

void WorldHandler::ResetTrees() {
    delete this->collision_tree;
    delete this->forces_tree;
    this->collision_tree = new CollisionOctree(cube_length * 2, min_coord_x, max_coord_x, min_coord_y, max_coord_y, min_coord_z, max_coord_z, true);
    this->forces_tree = new ForceOctree(cube_length * 5, min_coord_x, max_coord_x, min_coord_y, max_coord_y, min_coord_z, max_coord_z);

    // Add all blocks to the trees and store the leaves they were placed in (in the case of the CollisionOctree)

    for(auto const &world_block : this->iblocks) {
        auto temp = this->collision_tree->AddBlock(world_block);
        this->block_to_leaf[world_block] = temp;
        if(world_block->state) {
            this->forces_tree->AddIBlockPlus(world_block);
        } else {
            this->forces_tree->AddIBlockNeg(world_block);
        }
    }

    for(auto const &world_block : this->mblocks) {
        auto temp = this->collision_tree->AddBlock(world_block);
        this->block_to_leaf[world_block] = temp;
        if(world_block->flare_value > MBlock::threshold) {
            this->forces_tree->AddMBlockPlus(world_block);
        } else {
            this->forces_tree->AddMBlockNeg(world_block);
        }
    }

    for(auto const &world_block : this->eblocks) {
        auto temp = this->collision_tree->AddBlock(world_block);
        this->block_to_leaf[world_block] = temp;
        this->forces_tree->AddEBlock(world_block);
    }

    for(auto const &world_block : this->zblocks) {
        auto temp = this->collision_tree->AddBlock(world_block);
        this->block_to_leaf[world_block] = temp;
        this->forces_tree->AddZBlock(world_block);
    }

}

void WorldHandler::AddBlock(BlockTypes block_types, int num_blocks, bool state) {
        std::vector<Matrix> *positions, *linear_momentums;
        GetProperties(num_blocks, positions, linear_momentums, this->min_coord_x, this->max_coord_x, this->min_coord_y, this->max_coord_y,  this->min_coord_z, this->max_coord_z);
        CollisionOctree * tree_occupied;
        for(int i=0; i<num_blocks; i++) {
            if(block_types == IBlockType) {
                auto *new_block = new IBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0)  ,state);
                iblocks.push_back(new_block);
                if(state) {
                    tree_occupied = this->collision_tree->AddBlock(new_block);
                    this->forces_tree->AddIBlockPlus(new_block);
                } else {
                    tree_occupied = this->collision_tree->AddBlock(new_block);
                    this->forces_tree->AddIBlockNeg(new_block);
                }
                blocks.push_back(new_block);
            } else if(block_types == ZBlockType) {
                auto *new_block = new ZBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0));
                zblocks.push_back(new_block);
                tree_occupied = this->collision_tree->AddBlock(new_block);
                blocks.push_back(new_block);
                this->forces_tree->AddZBlock(new_block);
            } else if(block_types == EBlockType) {
                auto new_block = new EBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0), state);
                eblocks.push_back(new_block);
                tree_occupied = this->collision_tree->AddBlock(new_block);
                blocks.push_back(new_block);
                this->forces_tree->AddEBlock(new_block);
            } else {
                auto new_block = new MBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0));
                mblocks.push_back(new_block);
                tree_occupied = this->collision_tree->AddBlock(new_block);
                blocks.push_back(new_block);
                this->forces_tree->AddMBlockNeg(new_block);
            }
            auto temp = blocks.back();
            temp->SetLinearMomentum(linear_momentums->at(i));
            this->block_to_leaf[temp] = tree_occupied;
        };
        delete positions;
        delete linear_momentums;
}


void WorldHandler::Update(vector<Contact> &contact_list, real delta_time) {

    // We update the entire world
    // 1) We remove the block from the CollisionOctree
    // 2) We resolve all collisions that are currently taking place
    // 3) We then update the position of the block in the ForceOctree
    // 4) Update the position of the blocks


#pragma omp parallel for default(none)
    for(auto const &world_block : this->blocks) {
        collision_tree->RemoveBlock(world_block);
    }

    UpdateFlares(contact_list, delta_time);

#pragma omp parallel for default(none) shared(contact_list)
    for(auto & i : contact_list) {
//#pragma omp critical -- this shouldn't really be here???
        {
            Cube::CollisionResolution(i);
        }
    }


#pragma omp parallel for default(none) shared(delta_time)
    for(auto const &leaf_m_block: this->mblocks) {
        if(leaf_m_block->flare_value > MBlock::threshold) {
            forces_tree->RemoveMBlockPlus(leaf_m_block);
            leaf_m_block->Update(this->min_coord_x, this->max_coord_x, this->min_coord_y, this->max_coord_y, this->min_coord_z, this->max_coord_z, delta_time);
            CollisionOctree * new_leaf = collision_tree->AddBlock(leaf_m_block);
            block_to_leaf[leaf_m_block] = new_leaf;
            forces_tree->AddMBlockPlus(leaf_m_block);
        } else {
            forces_tree->RemoveMBlockNeg(leaf_m_block);
            leaf_m_block->Update(this->min_coord_x, this->max_coord_x, this->min_coord_y, this->max_coord_y, this->min_coord_z, this->max_coord_z, delta_time);
            CollisionOctree * new_leaf = collision_tree->AddBlock(leaf_m_block);
            block_to_leaf[leaf_m_block] = new_leaf;
            forces_tree->AddMBlockNeg(leaf_m_block);
        }

    }

#pragma omp parallel for default(none) shared(delta_time)
    for(auto const &leaf_i_block: this->iblocks) {
        if(leaf_i_block->state) {
            forces_tree->RemoveIBlockPlus(leaf_i_block);
            leaf_i_block->Update(this->min_coord_x, this->max_coord_x, this->min_coord_y, this->max_coord_y, this->min_coord_z, this->max_coord_z, delta_time);
            CollisionOctree * new_leaf = collision_tree->AddBlock(leaf_i_block);
            block_to_leaf[leaf_i_block] = new_leaf;
            forces_tree->AddIBlockPlus(leaf_i_block);
        } else {
            forces_tree->RemoveIBlockNeg(leaf_i_block);
            leaf_i_block->Update(this->min_coord_x, this->max_coord_x, this->min_coord_y, this->max_coord_y, this->min_coord_z, this->max_coord_z, delta_time);
            CollisionOctree * new_leaf = collision_tree->AddBlock(leaf_i_block);
            block_to_leaf[leaf_i_block] = new_leaf;
            forces_tree->AddIBlockNeg(leaf_i_block);
        }
    }

#pragma omp parallel for default(none) shared(delta_time)
    for(auto const &leaf_e_block: this->eblocks) {
        forces_tree->RemoveEBlock(leaf_e_block);
        leaf_e_block->Update(this->min_coord_x, this->max_coord_x, this->min_coord_y, this->max_coord_y, this->min_coord_z, this->max_coord_z, delta_time);
        CollisionOctree * new_leaf = collision_tree->AddBlock(leaf_e_block);
        block_to_leaf[leaf_e_block] = new_leaf;
        forces_tree->AddEBlock(leaf_e_block);
    }

#pragma omp parallel for default(none) shared(delta_time)
    for(auto const &leaf_z_block: this->zblocks) {
        forces_tree->RemoveZBlock(leaf_z_block);
        leaf_z_block->Update(this->min_coord_x, this->max_coord_x, this->min_coord_y, this->max_coord_y, this->min_coord_z, this->max_coord_z, delta_time);
        CollisionOctree * new_leaf = collision_tree->AddBlock(leaf_z_block);
        block_to_leaf[leaf_z_block] = new_leaf;
        forces_tree->AddZBlock(leaf_z_block);
    }
};

vector<Contact> WorldHandler::CollisionHandlerBruteForce()
{
    vector<Contact> contact_list;
    // Loop over every possible pair of block and determine if they are in direct collision
#pragma omp parallel for default(none) shared(contact_list)
    for(unsigned int i=0; i<blocks.size()-1; i++) {
        for(unsigned int j=i+1; j<blocks.size(); j++) {
            Cube::CollisionDetect(blocks.at(i), blocks.at(j), contact_list);
        }
    }
    return contact_list;
}

void WorldHandler::UpdateFlares(vector<Contact> &contact_list, real delta_time) {
    PassBlockFlares(contact_list, delta_time);
#pragma omp parallel for default(none) shared(delta_time)
    for(auto const &world_block : this->blocks) {
        world_block->Decay(delta_time);
    }
    SpinWorldBlocks();
    IncFlareValuesAndReset();
}

void WorldHandler::AddForces(real delta_time) {
    this->forces_tree->CalculateCOMonTree();
#pragma omp parallel for default(none) shared(delta_time)
    for(auto const &block : this->blocks) {
        if(!block->locked) {
            forces_tree->ApplyBarnesHutOnBlock(block, delta_time);
        }
    }
    // The below code is for testing purposes; To see if the Barnes Hut algorithm, gives accurate results
    // It is important to note, that the Barnes hut alogirthm approximates as compared to the below algorithm
    // that gives accurate values
/*#pragma omp parallel for
    for(auto &block0: this->blocks) {
        if (!block0->locked) {
            for (auto &block1: this->iblocks) {
                block0->ReactSerial(block1, delta_time);
            }
            for (auto &block1: this->zblocks) {
                block0->ReactSerial(block1, delta_time);
            }
            for (auto &block1: this->mblocks) {
                block0->ReactSerial(block1, delta_time);
            }
            for (auto &block1: this->eblocks) {
                block0->ReactSerial(block1, delta_time);
            }
        }
    }*/
}

void WorldHandler::PassBlockFlares(vector<Contact> &contacts, real delta_time) {
#pragma omp parallel for default(none) shared(contacts, delta_time)
    for(auto & contact : contacts) {
        PassFlare(contact.body1, contact.body2, delta_time);
    }
}

void WorldHandler::PassFlare(Block *a, Block *b, real deltatime) {

    real flare_from_a = a->ExtractFlareFromBlock(deltatime);
    real flare_from_b = b->ExtractFlareFromBlock(deltatime);
#pragma omp critical
    {
        a->AddFlareToBlock(flare_from_b, b);
        b->AddFlareToBlock(flare_from_a, a);
    };
}

void WorldHandler::IncFlareValuesAndReset() {
#pragma omp parallel for default(none)
   for(auto & block : this->blocks) {
        block->UpdateFlare();
        block->flare_inc = 0;
    }
}

void WorldHandler::AddSpin(vector<Block *> *block_list,  Matrix const &force_direction) {
#pragma omp parallel for default(none) shared(block_list, force_direction)
    for(auto & i : *block_list) {
        i->spin(force_direction);
    }
}

void WorldHandler::SpinWorldBlocks() {
    AddSpin((vector<Block*>*) (&this->mblocks), Matrix::CreateColumnVec(0.1, 0, 0));
    AddSpin((vector<Block*>*) (&this->iblocks), Matrix::CreateColumnVec(0, 0.01, 0));
    AddSpin((vector<Block*>*) (&this->eblocks), Matrix::CreateColumnVec(0, 0, 0.1));
}

WorldHandler::~WorldHandler() {
    delete this->collision_tree;
    delete this->forces_tree;

    for(auto & b : this->blocks) {
        delete b;
    }

}

#if defined(OPENMP)
std::unordered_set<CollisionOctree *> WorldHandler::GetBlockPositionsParallel() {
    // A separate one has been made for getting positions, since blocks could be clustered close together, so we may be able to take advantage of some spacial structure of the blocks
    float p = min(omp_get_max_threads(), (int) this->blocks.size()); // need to ensure the data is more than the number of threads

    unordered_set<CollisionOctree*> local_set[(int) p]; // An array of sets, where each index in the array is the local_set of a single thread

    int local_size = std::ceil(this->blocks.size()/p);
#pragma omp parallel for default(none) shared(p, local_size, local_set)
    for(int i=0; i<(int) p; i++){
        for(int j=i*local_size; j<min(local_size * (i+1), (int) this->blocks.size()); j++) {
            local_set[i].insert(this->block_to_leaf.at(this->blocks.at(j)));
        }
    }

    int num_threads_distributed_across = p;

    // We merge the sets together as we reduce to a single thread that will finally store every leaf.
    for(int i=0; i<ceil(log(p)); i++) {
        int t = floor(num_threads_distributed_across/2);
#pragma omp parallel for default(none) shared(num_threads_distributed_across, local_set, t)
        for(int j=0; j<t; j++) {
            for(const auto&elem : local_set[((int) floor(num_threads_distributed_across/2)) + j]) {
                local_set[j].insert(elem);
            }
            if(j == floor(num_threads_distributed_across/2)-1){
                if(num_threads_distributed_across % 2 == 1) {
                    for(const auto&elem : local_set[t + j + 1]) {
                        local_set[j].insert(elem);
                    }
                }
            }
        }
        num_threads_distributed_across = floor(num_threads_distributed_across/2);
    }
    return local_set[0];
}
#endif

void WorldHandler::MakeDAG(std::unordered_set<CollisionOctree *> &block_positions,
                           std::vector<pair<CollisionOctree *, CollisionOctree *>> &edges,
                           std::unordered_set<CollisionOctree *> &nodes) {
    for(auto const pos: block_positions) {
        if(nodes.find(pos) == nodes.end()) {
            GetDAGAtRoot(pos, edges, nodes);
        }
    }
}

void WorldHandler::GetDAGAtRoot(CollisionOctree *root, std::vector<pair<CollisionOctree *, CollisionOctree *>> &edges,
                                std::unordered_set<CollisionOctree *> &nodes) {
    if(nodes.find(root) != nodes.end()) {
        return;
    }

    auto children = this->collision_tree->grid_elements_neighbours.at(root);
    nodes.insert(root);

    for(auto & child : children) {
        if (!child->blocks_at_leaf.empty() && nodes.find(child) == nodes.end()) {
            {
                edges.emplace_back(root, child);
            }
        }
    }

    for(auto & child : children) {
        if(!child->blocks_at_leaf.empty() && nodes.find(child) == nodes.end()) {
            GetDAGAtRoot(child, edges, nodes);
        }
    }
}

vector<Contact> WorldHandler::CollisionHandlerWithOctree() {

#if defined(OPENMP)
    std::unordered_set<CollisionOctree *> pos = GetBlockPositionsParallel();

    /*std::unordered_set<CollisionOctree *> pos1 = GetBlockPositionsSerial();
    if(pos != pos1) {
       cout << "not equalllllll" << endl;
       cout << pos.size() << " " << pos1.size() << endl;
    }*/
#else
    std::unordered_set<CollisionOctree *> pos = GetBlockPositionsSerial();
#endif

    std::vector<pair<CollisionOctree *, CollisionOctree *>> edges;
    std::unordered_set<CollisionOctree *> nodes;
    MakeDAG(pos, edges, nodes);

    vector<Contact> collisions;

    /* We loop edges of the DAG, where the edges of the DAG represent a tuple (N1, N2). We then
    compute collisions of all blocks (A, B), such that A resides in the Node N1 and B resides in the node N2
    */
#pragma omp parallel for default(none) shared(edges, collisions)
    for(auto const &edge: edges) {
        CollisionOctree *o1 = edge.first;
        CollisionOctree *o2 = edge.second;
        if(o1 == o2) {
            continue;
        }
        for(auto const &b1: o1->blocks_at_leaf) {
            for(auto const &b2: o2->blocks_at_leaf) {

                Cube::CollisionDetect(b1, b2, collisions);
                //Cube::CollisionDetectAndResolve(b1, b2, this->collision_locks);
            }
        }
    }

    // We now compute collisions of blocks with other blocks that share the same leaf
    vector<CollisionOctree *> single_leaves;
    for(auto const &p: pos) {
        single_leaves.push_back(p);
    }

#pragma omp parallel for default(none) shared(pos, collisions, single_leaves)
    for(auto const &p: single_leaves) {
        vector<Block *> v;
        for(auto const &b : p->blocks_at_leaf) {
            v.push_back(b);
        }
        for(unsigned int i=0; i<v.size()-1; i++) {
            for(unsigned int j=i+1;j<v.size();j++) {
                Cube::CollisionDetect(v.at(i), v.at(j), collisions);
                //Cube::CollisionDetectAndResolve(v.at(i), v.at(j), this->collision_locks);
            }
        }
    }
    return collisions;
}

std::unordered_set<CollisionOctree *> WorldHandler::GetBlockPositionsSerial() {
    std::unordered_set<CollisionOctree *> r;
    for(auto & block : this->blocks) {
        r.insert(this->block_to_leaf[block]);
    }
    return r;
}
