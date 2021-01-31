#include <cstdlib>
#include <ctime>
#include <vector>

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
    auto get_momentums = []() -> real { return (rand() % 1)/100 - 0.0005;};
    for (int i = 0; i < num_cubes; i++)
    {
        real values[] = { get_momentums() , get_momentums() , get_momentums()};
        linear_momentums->push_back(Matrix(3, 1, values));
    }
    return linear_momentums;
}

WorldHandler::WorldHandler(int num_i_blocks_plus, int num_i_blocks_neg, int num_z_blocks, int num_m_blocks, int num_e_blocks_1, int num_e_blocks_1_2,
                           real min_coord, real max_coord) {
    srand((unsigned)time(0)); //NULL???

    this->tree = new Octree(4, min_coord, max_coord, min_coord, max_coord, min_coord, max_coord);
    this->world_size = max_coord - min_coord;
    this->max_coord = max_coord;
    this->min_coord = min_coord;

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
        for(int i=0; i<num_blocks; i++) {
            if(block_types == IBlockType) {
                auto *new_block = new IBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0)  ,state);
                iblocks.push_back(new_block);
                blocks.push_back(new_block);
            } else if(block_types == ZBlockType) {
                auto *new_block = new ZBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0));
                zblocks.push_back(new_block);
                blocks.push_back(new_block);
            } else if(block_types == EBlockType) {
                auto new_block = new EBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0), state);
                eblocks.push_back(new_block);
                blocks.push_back(new_block);
            } else {
                auto new_block =  new MBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0));
                mblocks.push_back(new_block);
                blocks.push_back(new_block);
            }
            blocks.back()->SetLinearMomentum(linear_momentums->at(i));
            this->tree->AddBlock(blocks.back(), i);
    }
}


void WorldHandler::Update() {
    for(int i=0; i<this->blocks.size(); i++) {
        this->blocks.at(i)->Update();
    }
};


void WorldHandler::CollisionHandler(real deltatime) {
    vector<Contact> contact_list;
    for(int i=0; i<blocks.size()-1; i++) {
        for(int j=i+1; j<blocks.size(); j++) {
            Cube::CollisionDetect(blocks.at(i), blocks.at(j), contact_list);
        }
    }
    for(int i=0; i<contact_list.size(); i++) {
        Cube::CollisionResolution(contact_list.at(i));
    }
    PassBlockFlares(contact_list, deltatime);
    SpinWorldBlocks();
    IncFlareValuesAndReset();
}

void WorldHandler::AddForces(real deltatime) {
    for(auto &block: this->blocks) {
        ReactToAllBlocks(block, deltatime * 60);
    }
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