#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include <set>
#include <omp.h>

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
        linear_momentums->push_back(Matrix(3, 1, values));
    }
    return linear_momentums;
}

WorldHandler::WorldHandler(int num_i_blocks_plus, int num_i_blocks_neg, int num_z_blocks, int num_m_blocks, int num_e_blocks_1, int num_e_blocks_1_2,
                           real min_coord, real max_coord, real cube_length) {
    srand((unsigned)time(0)); //NULL???

    cout << "Quad trees are being initialised" << endl;
    this->tree = new Octree(cube_length * 2, min_coord, max_coord, min_coord, max_coord, min_coord, max_coord, true);
    this->forces_tree = new ForceOctree(cube_length * 5, min_coord, max_coord, min_coord, max_coord, min_coord, max_coord,
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

#pragma omp parallel for
    for(auto const &world_block : this->blocks) {
        tree->RemoveBlock(world_block);
    }

    UpdateFlares(contact_list, delta_time);

#pragma omp parallel for
    for(unsigned int i=0; i<contact_list.size(); i++) {
#pragma omp critical
        {
            Cube::CollisionResolution(contact_list.at(i));
        }
    }

#pragma omp parallel for
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

#pragma omp parallel for
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

#pragma omp parallel for
    for(auto const &leaf_e_block: this->eblocks) {
        forces_tree->RemoveEBlock(leaf_e_block);
        leaf_e_block->Update(this->min_coord, this->max_coord, this->min_coord, this->max_coord, this->min_coord, this->max_coord, delta_time);
        Octree * new_leaf = tree->AddBlock(leaf_e_block);
        block_to_leaf[leaf_e_block] = new_leaf;
        forces_tree->AddEBlock(leaf_e_block);
    }

#pragma omp parallel for
    for(auto const &leaf_z_block: this->zblocks) {
        forces_tree->RemoveZBlock(leaf_z_block);
        leaf_z_block->Update(this->min_coord, this->max_coord, this->min_coord, this->max_coord, this->min_coord, this->max_coord, delta_time);
        Octree * new_leaf = tree->AddBlock(leaf_z_block);
        block_to_leaf[leaf_z_block] = new_leaf;
        forces_tree->AddZBlock(leaf_z_block);
    }
};

vector<Contact> WorldHandler::CollisionHandler()
{


    // When testing if both methods of calculating collisions return the same value,
    // check CollisionDetect function and comment out if statement, if(dist==0), in CollisionDetect

    vector<Contact> contact_list;
    set<pair<Block *, Block *>> collisions_checked;


#pragma omp parallel for
    for(auto const &collision_blocks : this->blocks) {
        Octree *coll_tree = this->block_to_leaf[collision_blocks];
        vector<Octree *> neighbour_nodes = this->tree->grid_elements_neighbours[coll_tree];
        for(auto const &n: neighbour_nodes) {
            for(auto const &node_in_n: n->blocks_at_leaf) {
                auto f1 = collisions_checked.find(make_pair(node_in_n, collision_blocks));
                auto f2 = collisions_checked.find(make_pair(collision_blocks, node_in_n));

                if(f1 == collisions_checked.end() && f2 == collisions_checked.end()) {
                    if(collision_blocks == node_in_n) {
                        continue;
                    }
                    Cube::CollisionDetect(collision_blocks,
                                          node_in_n,
                                          contact_list);
#pragma omp critical
                    {
                        collisions_checked.insert(make_pair(collision_blocks, node_in_n));
                    };
                }
            }
        }
    }

    vector<Contact> contact_list_test;
#pragma omp parallel for
    for(unsigned int i=0; i<blocks.size()-1; i++) {
        for(unsigned int j=i+1; j<blocks.size(); j++) {
            Cube::CollisionDetect(blocks.at(i), blocks.at(j), contact_list_test);
        }
    }
    //return contact_list_test;

    set<pair<Block *, Block *>> c;
    set<pair<Block *, Block *>> c1;

    for(auto const &k : contact_list) {
        c.insert(make_pair(k.body1, k.body2));
        c.insert(make_pair(k.body2, k.body1));
    }

    for(auto const &k : contact_list_test) {
        c1.insert(make_pair(k.body1, k.body2));
        c1.insert(make_pair(k.body2, k.body1));
    }


    if(c != c1 || contact_list.size() != contact_list_test.size()) {
        cout << c.size() << endl;
        cout << c1.size() << endl;
        cout << "not worked" << endl;
    }

    return contact_list;
}

void WorldHandler::UpdateFlares(vector<Contact> &contact_list, real delta_time) {
    PassBlockFlares(contact_list, delta_time);
#pragma omp parallel for
    for(auto const &world_block : this->blocks) {
        world_block->Decay(delta_time);
    }
    SpinWorldBlocks();
    IncFlareValuesAndReset();
}

void WorldHandler::AddForces(real deltatime) {
    this->forces_tree->CalculateCOMonTree();
#pragma omp parallel for
    for(auto const &block : this->blocks) {
        if(!block->locked) {
            forces_tree->ApplyBarnesHutOnBlock(block, deltatime);
        }
    }
}

void WorldHandler::PassBlockFlares(vector<Contact> &contacts, real deltatime) {
#pragma omp parallel for
    for(auto & contact : contacts) {
        PassFlare(contact.body1, contact.body2, deltatime);
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
#pragma omp parallel for
   for(auto & block : this->blocks) {
        block->UpdateFlare();
        block->flare_inc = 0;
    }
}

void WorldHandler::AddSpin(vector<Block *> *block_list,  Matrix const &force_direction) {
#pragma omp parallel for
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
