#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>

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
    auto get_momentums = []() -> real { return (rand() % 1)/100 - 0.005;};
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

    cout << "Quad tree is Being initialised" << endl;
    this->tree = new Octree(cube_length * 2, min_coord, max_coord, min_coord, max_coord, min_coord, max_coord, true);
    cout << "Quad tree initialised" << endl;
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


void WorldHandler::AddBlock(BlockTypes block_types, int num_blocks, bool state) {
        std::vector<Matrix> *positions, *linear_momentums;
        GetProperties(num_blocks, positions, linear_momentums, this->min_coord, this->max_coord);
        Octree * tree_occupied;
        for(int i=0; i<num_blocks; i++) {
            if(block_types == IBlockType) {
                auto *new_block = new IBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0)  ,state);
                iblocks.push_back(new_block);
                tree_occupied = this->tree->AddIBlock(new_block);
                blocks.push_back(new_block);
            } else if(block_types == ZBlockType) {
                auto *new_block = new ZBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0));
                zblocks.push_back(new_block);
                tree_occupied = this->tree->AddZBlock(new_block);
                blocks.push_back(new_block);
            } else if(block_types == EBlockType) {
                auto new_block = new EBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0), state);
                eblocks.push_back(new_block);
                tree_occupied = this->tree->AddEBlock(new_block);
                blocks.push_back(new_block);
            } else {
                auto new_block =  new MBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0));
                mblocks.push_back(new_block);
                tree_occupied = this->tree->AddMBlock(new_block);
                blocks.push_back(new_block);
            }
            auto temp = blocks.back();
            temp->SetLinearMomentum(linear_momentums->at(i));
            this->block_to_leaf[temp] = tree_occupied;
    }
}


void WorldHandler::Update() {

    for(int i=0; i<this->blocks.size(); i++) {
        this->blocks.at(i)->Update(this->min_coord, this->max_coord, this->min_coord, this->max_coord, this->min_coord, this->max_coord);
    }


    for(auto const &leaf_i_block: this->iblocks) {
        Octree *leaf = block_to_leaf[leaf_i_block];
        if(!Octree::BlockInCorrectTree(leaf, leaf_i_block)) {
            leaf->RemoveIBlock(leaf_i_block);
            tree->AddIBlock(leaf_i_block);
            block_to_leaf[leaf_i_block] = tree->AddIBlock(leaf_i_block);
        }
    }

    for(auto const &leaf_m_block: this->mblocks) {
        Octree *leaf = block_to_leaf[leaf_m_block];
        if(!Octree::BlockInCorrectTree(leaf, leaf_m_block)) {
            leaf->RemoveMBlock(leaf_m_block);
            tree->AddMBlock(leaf_m_block);
            block_to_leaf[leaf_m_block] = tree->AddMBlock(leaf_m_block);
        }
    }

    for(auto const &leaf_z_block: this->zblocks) {
        Octree *leaf = block_to_leaf[leaf_z_block];
        if (!Octree::BlockInCorrectTree(leaf, leaf_z_block)) {
            leaf->RemoveZBlock(leaf_z_block);
            tree->AddZBlock(leaf_z_block);
            block_to_leaf[leaf_z_block] = tree->AddZBlock(leaf_z_block);
        }
    }

    for(auto const &leaf_e_block: this->eblocks) {
        Octree *leaf = block_to_leaf[leaf_e_block];
        if(!Octree::BlockInCorrectTree(leaf, leaf_e_block)) {
            leaf->RemoveEBlock(leaf_e_block);
            tree->AddEBlock(leaf_e_block);
            block_to_leaf[leaf_e_block] = tree->AddEBlock(leaf_e_block);
        }
    }

};


void WorldHandler::CollisionHandler(real deltatime) {

    /*vector<Contact> contact_list_test;
    for(int i=0; i<blocks.size()-1; i++) {
        for(int j=i+1; j<blocks.size(); j++) {
            Cube::CollisionDetect(blocks.at(i), blocks.at(j), contact_list_test);
        }
    }*/

    vector<Contact> contact_list;
    set<pair<Block *, Block *>> collisions_checked;
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
                    collisions_checked.insert(make_pair(collision_blocks, node_in_n));
                }
            }
        }
    }

    /*set<pair<Block *, Block *>> c;
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
    }*/

    for(int i=0; i<contact_list.size(); i++) {
        Cube::CollisionResolution(contact_list.at(i));
    }
    PassBlockFlares(contact_list, deltatime);
    for(int i=0; i<this->blocks.size(); i++) {
        Cube::CollisionBoundary(this->blocks.at(i), this->min_coord, this->max_coord, this->min_coord, this->max_coord,
                                this->min_coord, this->max_coord);
    }
    SpinWorldBlocks();
    IncFlareValuesAndReset();
}


void WorldHandler::CalculateCOM() {
    for(auto const &leaf : this->trees_occupied) {
        real si = leaf->iblocks_at_leaf.size();
        real sm = leaf->mblocks_at_leaf.size();
        real sz = leaf->zblocks_at_leaf.size();
        real se = leaf->eblocks_at_leaf.size();
        this->i_leafs_occupied_com[leaf] = make_tuple(leaf->sum_x_i/si, leaf->sum_y_i/se, leaf->sum_z_i/se);
        this->m_leafs_occupied_com[leaf] = make_tuple(leaf->sum_x_m/sm, leaf->sum_y_m/sm, leaf->sum_z_m/sm);
        this->z_leafs_occupied_com[leaf] = make_tuple(leaf->sum_x_z/sz, leaf->sum_y_z/sz, leaf->sum_z_z/sz);
        this->e_leafs_occupied_com[leaf] = make_tuple(leaf->sum_x_e/se, leaf->sum_y_e/se, leaf->sum_z_e/se);
    }
}

void WorldHandler::AddForces(real deltatime) {
    for(auto &block: this->blocks) {
        ReactToAllBlocks(block, deltatime * 10);
    }

    /*deltatime *=10;
    for(auto &x_block_counter: this->iblocks) {
        auto x = x_block_counter->position(0, 0);
        auto y = x_block_counter->position(1, 0);
        auto z = x_block_counter->position(2, 0);
        Octree *t = this->tree->GetGridAtPos(x, y, z);
        vector<Octree *> neighbours = this->tree->grid_elements_neighbours[t];

        for(int i=0; i<neighbours.size(); i++) {
            for(auto &n : neighbours.at(i)->blocks_at_leaf) {

                Matrix to_cube = x_block_counter->position - n.second->position;
                real squared_dist = Matrix::SquaredNorm(to_cube);
                if(squared_dist >= 25) {
                    n.second->React(x_block_counter, squared_dist, to_cube, deltatime);
                }
            }
        }
    }


    for(auto &x_block_counter: this->mblocks) {
        auto x = x_block_counter->position(0, 0);
        auto y = x_block_counter->position(1, 0);
        auto z = x_block_counter->position(2, 0);
        Octree *t = this->tree->GetGridAtPos(x, y, z);
        vector<Octree *> neighbours = this->tree->grid_elements_neighbours[t];

        for(int i=0; i<neighbours.size(); i++) {
            for(auto &n : neighbours.at(i)->blocks_at_leaf) {

                Matrix to_cube = x_block_counter->position - n.second->position;
                real squared_dist = Matrix::SquaredNorm(to_cube);
                if(squared_dist >= 25) {
                    n.second->React(x_block_counter, squared_dist, to_cube, deltatime);
                }
            }
        }
    }


    for(auto &x_block_counter: this->zblocks) {
        auto x = x_block_counter->position(0, 0);
        auto y = x_block_counter->position(1, 0);
        auto z = x_block_counter->position(2, 0);
        Octree *t = this->tree->GetGridAtPos(x, y, z);
        vector<Octree *> neighbours = this->tree->grid_elements_neighbours[t];

        for(int i=0; i<neighbours.size(); i++) {
            for(auto &n : neighbours.at(i)->blocks_at_leaf) {

                Matrix to_cube = x_block_counter->position - n.second->position;
                real squared_dist = Matrix::SquaredNorm(to_cube);
                if(squared_dist >= 25) {
                    n.second->React(x_block_counter, squared_dist, to_cube, deltatime);
                }
            }
        }
    }

    for(auto &x_block_counter: this->eblocks) {
        auto x = x_block_counter->position(0, 0);
        auto y = x_block_counter->position(1, 0);
        auto z = x_block_counter->position(2, 0);
        Octree *t = this->tree->GetGridAtPos(x, y, z);
        vector<Octree *> neighbours = this->tree->grid_elements_neighbours[t];

        for(int i=0; i<neighbours.size(); i++) {
            for(auto &n : neighbours.at(i)->blocks_at_leaf) {

                Matrix to_cube = x_block_counter->position - n.second->position;
                real squared_dist = Matrix::SquaredNorm(to_cube);
                if(squared_dist >= 25) {
                    n.second->React(x_block_counter, squared_dist, to_cube, deltatime);
                }
            }
        }
    } */

}

void WorldHandler::ReactToAllBlocks(Block *block, real deltatime) {
    for(int i=0; i<iblocks.size(); i++) {
        Matrix to_cube = iblocks.at(i)->position - block->position;
        real squared_dist = Matrix::SquaredNorm(to_cube);
        if(squared_dist >= 25) {
            block->React(iblocks.at(i), squared_dist, to_cube, deltatime);
        }
    }

    for(int i=0; i<zblocks.size(); i++) {
        Matrix to_cube = zblocks.at(i)->position - block->position;
        real squared_dist = Matrix::SquaredNorm(to_cube);
        if(squared_dist >= 25) {
            block->React(zblocks.at(i), squared_dist, to_cube, deltatime);
        }
    }

    for(int i=0; i<eblocks.size(); i++) {
        Matrix to_cube = eblocks.at(i)->position - block->position;
        real squared_dist = Matrix::SquaredNorm(to_cube);
        if(squared_dist >= 25) {
            block->React(eblocks.at(i), squared_dist, to_cube, deltatime);
        }
    }

    for(int i=0; i<mblocks.size(); i++) {
        Matrix to_cube = mblocks.at(i)->position - block->position;
        real squared_dist = Matrix::SquaredNorm(to_cube);
        if(squared_dist >= 25) {
            block->React(mblocks.at(i), squared_dist, to_cube, deltatime);
        }
    }
}

void WorldHandler::PassBlockFlares(vector<Contact> &contacts, real deltatime) {
    for(int i=0; i<contacts.size(); i++) {
        PassFlare(contacts.at(i).body1, contacts.at(i).body2, deltatime);
    }
}

void WorldHandler::PassFlare(Block *a, Block *b, real deltatime) {
    real flare_from_a = a->ExtractFlareFromBlock(deltatime);
    real flare_from_b = b->ExtractFlareFromBlock(deltatime);

    a->AddFlareToBlock(flare_from_b);
    b->AddFlareToBlock(flare_from_a);
}

void WorldHandler::IncFlareValuesAndReset() {
    for(int i=0; i<this->blocks.size(); i++) {
        this->blocks.at(i)->UpdateFlare();
        this->blocks.at(i)->flare_inc = 0;
    }
}

void WorldHandler::AddSpin(vector<Block *> *block_list,  Matrix const &force_direction) {
    for(int i=0; i<block_list->size(); i++) {
        block_list->at(i)->spin(force_direction);
    }
}

void WorldHandler::SpinWorldBlocks() {
    real const static force_direction_1[] = {5, 0, 0};
    real const static force_direction_2[] = {0, 5, 0};
    real const static force_direction_3[] = {0, 0, 5};

    Matrix const static force_direction_mat1(3, 1, force_direction_1);
    Matrix const static force_direction_mat2(3, 1, force_direction_2);
    Matrix const static force_direction_mat3(3, 1, force_direction_3);

    AddSpin((vector<Block*>*) (&this->mblocks), force_direction_mat1);
    AddSpin((vector<Block*>*) (&this->iblocks), force_direction_mat2);
    AddSpin((vector<Block*>*) (&this->eblocks), force_direction_mat3);
}