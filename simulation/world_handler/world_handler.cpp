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

std::vector<Matrix> *WorldHandler::GeneratePositions(int num_cubes, real min_coord, real max_coord)
{
    auto *positions = new std::vector<Matrix>;
    int world_size = (int) (max_coord - min_coord);
    auto get_coord = [world_size, min_coord]() -> real { return (rand()%( world_size + 1) + min_coord); };
    for (int i = 0; i < num_cubes; i++)
    {
        real values[] = {get_coord(), get_coord(), get_coord()};
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
                           real min_coord, real max_coord, real cube_length) {
    srand((unsigned)time(0));
    //srand(0); // useful for testing purposes, to generate the same random numbers

    cout << "Quad trees are being initialised" << endl;
    this->tree = new Octree((int)cube_length * 3, min_coord, max_coord, min_coord, max_coord, min_coord, max_coord, true);
    this->forces_tree = new ForceOctree((int) cube_length * 5, min_coord, max_coord, min_coord, max_coord, min_coord, max_coord,
                                   false);
    cout << "Quad trees are initialised" << endl;
    this->world_size = max_coord - min_coord;
    this->max_coord = max_coord;
    this->min_coord = min_coord;
    this->cube_length = cube_length;

    AddBlock(IBlockType, num_i_blocks_plus, true);
    AddBlock(IBlockType, num_i_blocks_neg, false);
    AddBlock(MBlockType, num_m_blocks, true);
    AddBlock(EBlockType, num_e_blocks_1, true);
    AddBlock(EBlockType, num_e_blocks_1_2, false);
    AddBlock(ZBlockType, num_z_blocks, true);

#if defined(OPENMP)
    for(unsigned int i=0; i<this->blocks.size(); i++) {
       this->collision_locks.emplace_back(omp_lock_t());
       omp_init_lock(&this->collision_locks.at(i));
    }
#endif
}

void WorldHandler::GetProperties(int num_blocks, std::vector<Matrix> *&positions, std::vector<Matrix> * &linear_momentums, real min_coord, real max_coord) {
    positions = GeneratePositions(num_blocks, min_coord, max_coord);
    linear_momentums = GenerateLinearMomentums(num_blocks);
}

void WorldHandler::ResetTrees() {
    delete this->tree;
    delete this->forces_tree;
    this->tree = new Octree(cube_length * 2, min_coord, max_coord, min_coord, max_coord, min_coord, max_coord, true);
    this->forces_tree = new ForceOctree(cube_length * 5, min_coord, max_coord, min_coord, max_coord, min_coord, max_coord,
                                        false);

    for(auto const &world_block : this->iblocks) {
        auto temp = this->tree->AddBlock(world_block);
        this->block_to_leaf[world_block] = temp;
        if(world_block->state) {
            this->forces_tree->AddIBlockPlus(world_block);
        } else {
            this->forces_tree->AddIBlockNeg(world_block);
        }
    }

    for(auto const &world_block : this->mblocks) {
        auto temp = this->tree->AddBlock(world_block);
        this->block_to_leaf[world_block] = temp;
        if(world_block->flare_value > MBlock::threshold) {
            this->forces_tree->AddMBlockPlus(world_block);
        } else {
            this->forces_tree->AddMBlockNeg(world_block);
        }
    }

    for(auto const &world_block : this->eblocks) {
        auto temp = this->tree->AddBlock(world_block);
        this->block_to_leaf[world_block] = temp;
        this->forces_tree->AddEBlock(world_block);
    }

    for(auto const &world_block : this->zblocks) {
        auto temp = this->tree->AddBlock(world_block);
        this->block_to_leaf[world_block] = temp;
        this->forces_tree->AddZBlock(world_block);
    }

}

void WorldHandler::AddBlock(BlockTypes block_types, int num_blocks, bool state) {
        std::vector<Matrix> *positions, *linear_momentums;
        GetProperties(num_blocks, positions, linear_momentums, this->min_coord, this->max_coord);
        Octree * tree_occupied;
        for(int i=0; i<num_blocks; i++) {
            if(block_types == IBlockType) {
                auto *new_block = new IBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0)  ,state);
                iblocks.push_back(new_block);
                if(state) {
                    tree_occupied = this->tree->AddBlock(new_block);
                    this->forces_tree->AddIBlockPlus(new_block);
                } else {
                    tree_occupied = this->tree->AddBlock(new_block);
                    this->forces_tree->AddIBlockNeg(new_block);
                }
                blocks.push_back(new_block);
            } else if(block_types == ZBlockType) {
                auto *new_block = new ZBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0));
                zblocks.push_back(new_block);
                tree_occupied = this->tree->AddBlock(new_block);
                blocks.push_back(new_block);
                this->forces_tree->AddZBlock(new_block);
            } else if(block_types == EBlockType) {
                auto new_block = new EBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0), state);
                eblocks.push_back(new_block);
                tree_occupied = this->tree->AddBlock(new_block);
                blocks.push_back(new_block);
                this->forces_tree->AddEBlock(new_block);
            } else {
                auto new_block = new MBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0));
                mblocks.push_back(new_block);
                tree_occupied = this->tree->AddBlock(new_block);
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

#pragma omp parallel for default(none)
    for(auto const &world_block : this->blocks) {
        tree->RemoveBlock(world_block);
    }

    UpdateFlares(contact_list, delta_time);

#pragma omp parallel for default(none) shared(contact_list)
    for(auto & i : contact_list) {
#pragma omp critical
        {
            Cube::CollisionResolution(i);
        }
    }


#pragma omp parallel for default(none) shared(delta_time)
    for(auto const &leaf_m_block: this->mblocks) {
        if(leaf_m_block->flare_value > MBlock::threshold) {
            forces_tree->RemoveMBlockPlus(leaf_m_block);
            leaf_m_block->Update(this->min_coord, this->max_coord, this->min_coord, this->max_coord, this->min_coord, this->max_coord, delta_time);
            Octree * new_leaf = tree->AddBlock(leaf_m_block);
            block_to_leaf[leaf_m_block] = new_leaf;
            forces_tree->AddMBlockPlus(leaf_m_block);
        } else {
            forces_tree->RemoveMBlockNeg(leaf_m_block);
            leaf_m_block->Update(this->min_coord, this->max_coord, this->min_coord, this->max_coord, this->min_coord, this->max_coord, delta_time);
            Octree * new_leaf = tree->AddBlock(leaf_m_block);
            block_to_leaf[leaf_m_block] = new_leaf;
            forces_tree->AddMBlockNeg(leaf_m_block);
        }

    }

#pragma omp parallel for default(none) shared(delta_time)
    for(auto const &leaf_i_block: this->iblocks) {
        if(leaf_i_block->state) {
            forces_tree->RemoveIBlockPlus(leaf_i_block);
            leaf_i_block->Update(this->min_coord, this->max_coord, this->min_coord, this->max_coord, this->min_coord, this->max_coord, delta_time);
            Octree * new_leaf = tree->AddBlock(leaf_i_block);
            block_to_leaf[leaf_i_block] = new_leaf;
            forces_tree->AddIBlockPlus(leaf_i_block);
        } else {
            forces_tree->RemoveIBlockNeg(leaf_i_block);
            leaf_i_block->Update(this->min_coord, this->max_coord, this->min_coord, this->max_coord, this->min_coord, this->max_coord, delta_time);
            Octree * new_leaf = tree->AddBlock(leaf_i_block);
            block_to_leaf[leaf_i_block] = new_leaf;
            forces_tree->AddIBlockNeg(leaf_i_block);
        }
    }

#pragma omp parallel for default(none) shared(delta_time)
    for(auto const &leaf_e_block: this->eblocks) {
        forces_tree->RemoveEBlock(leaf_e_block);
        leaf_e_block->Update(this->min_coord, this->max_coord, this->min_coord, this->max_coord, this->min_coord, this->max_coord, delta_time);
        Octree * new_leaf = tree->AddBlock(leaf_e_block);
        block_to_leaf[leaf_e_block] = new_leaf;
        forces_tree->AddEBlock(leaf_e_block);
    }

#pragma omp parallel for default(none) shared(delta_time)
    for(auto const &leaf_z_block: this->zblocks) {
        forces_tree->RemoveZBlock(leaf_z_block);
        leaf_z_block->Update(this->min_coord, this->max_coord, this->min_coord, this->max_coord, this->min_coord, this->max_coord, delta_time);
        Octree * new_leaf = tree->AddBlock(leaf_z_block);
        block_to_leaf[leaf_z_block] = new_leaf;
        forces_tree->AddZBlock(leaf_z_block);
    }
};

vector<Contact> WorldHandler::CollisionHandlerBruteForce()
{
    vector<Contact> contact_list;
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
    delete this->tree;
    delete this->forces_tree;

    for(auto & b : this->blocks) {
        delete b;
    }

}


#if defined(OPENMP)
template<typename T>
std::unordered_set<T> WorldHandler::VecToSetParallel(vector<T> v) {
    // sort out this yellow stuff
    float p = min(omp_get_max_threads(), (int) v.size()); // need to ensure the data is more than the number of threads
    unordered_set<T> local_set[(int) p];

    int local_size = std::ceil(v.size()/p);
#pragma omp parallel for
    for(int i=0; i<p; i++){
        for(int j=i*local_size; j<min(local_size * (i+1), (int) v.size()); j++) {
            local_set[i].insert(v.at(j));
        }
    }

    int num_threads_distributed_across = p; // p=4
    for(int i=0; i<ceil(log(p)); i++) { // 2 {0, 1}
        int t = floor(num_threads_distributed_across/2);
#pragma omp parallel for
        for(int j=0; j<t; j++) { // 2 {0, 1}
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



#if defined(OPENMP)
std::unordered_set<Octree *> WorldHandler::GetBlockPositionsParallel() {
    // A separate one has been made for getting positions, since blocks could be clustered close together, so we may be able to take advantage of some spacial structure of the blocks
    float p = min(omp_get_max_threads(), (int) this->blocks.size()); // need to ensure the data is more than the number of threads
    unordered_set<Octree*> local_set[(int) p];

    int local_size = std::ceil(this->blocks.size()/p);
#pragma omp parallel for default(none) shared(p, local_size, local_set)
    for(int i=0; i<(int) p; i++){
        for(int j=i*local_size; j<min(local_size * (i+1), (int) this->blocks.size()); j++) {
            local_set[i].insert(this->block_to_leaf.at(this->blocks.at(j)));
        }
    }

    int num_threads_distributed_across = p; // p=4
    for(int i=0; i<ceil(log(p)); i++) { // 2 {0, 1}
        int t = floor(num_threads_distributed_across/2);
#pragma omp parallel for default(none) shared(num_threads_distributed_across, local_set, t)
        for(int j=0; j<t; j++) { // 2 {0, 1}
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

void WorldHandler::MakeDAG(std::unordered_set<Octree *> &block_positions,
                           std::vector<pair<Octree *, Octree *>> &edges,
                           std::unordered_set<Octree *> &nodes) {
    for(auto const pos: block_positions) {
        if(nodes.find(pos) == nodes.end()) {
            GetDAGAtRoot(pos, edges, nodes);
        }
    }
}

void WorldHandler::GetDAGAtRoot(Octree *root, std::vector<pair<Octree *, Octree *>> &edges,
                                std::unordered_set<Octree *> &nodes) {
    if(nodes.find(root) != nodes.end()) {
        return;
    }

    auto children = this->tree->grid_elements_neighbours.at(root);
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
    std::unordered_set<Octree *> pos = GetBlockPositionsParallel();

    /*std::unordered_set<Octree *> pos1 = GetBlockPositionsSerial();
    if(pos != pos1) {
       cout << "not equalllllll" << endl;
       cout << pos.size() << " " << pos1.size() << endl;
    }*/
#else
    std::unordered_set<Octree *> pos = GetBlockPositionsSerial();
#endif

    std::vector<pair<Octree *, Octree *>> edges;
    std::unordered_set<Octree *> nodes;
    MakeDAG(pos, edges, nodes);

    vector<Contact> collisions;

#pragma omp parallel for default(none) shared(edges, collisions)
    for(auto const &edge: edges) {
        Octree *o1 = edge.first;
        Octree *o2 = edge.second;
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

    vector<Octree *> bb;
    for(auto const &p: pos) {
        bb.push_back(p);
    }

#pragma omp parallel for default(none) shared(pos, collisions, bb)
    for(auto const &p: bb) {
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

std::unordered_set<Octree *> WorldHandler::GetBlockPositionsSerial() {
    std::unordered_set<Octree *> r;
    for(auto & block : this->blocks) {
        r.insert(this->block_to_leaf[block]);
    }
    return r;
}
