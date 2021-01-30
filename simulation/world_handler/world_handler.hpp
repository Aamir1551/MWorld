#ifndef MWORLD_SIMULATION_WORLD_HANDLER_H
#define MWORLD_SIMULATION_WORLD_HANDLER_H

#include <cube.hpp>

#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

#include <vector>

#include <block.hpp>
#include <i_block.hpp>
#include <m_block.hpp>
#include <e_block.hpp>
#include <z_block.hpp>

#include <matrix.hpp>

using namespace blocks;

std::vector<Matrix> *GeneratePositions(int num_cubes)
{
    auto *positions = new std::vector<Matrix>;
    real world_size = 100.0;
    real const scale = world_size / ((real)RAND_MAX / 2.0);
    auto get_coord = [scale, world_size]() -> real { return scale * (rand() - RAND_MAX / 2.0); };
    for (int i = 0; i < num_cubes; i++)
    {
        real values[] = {get_coord(), get_coord(), get_coord()};
        positions->push_back(Matrix(3, 1, values));
    }
    return positions;
}

std::vector<Matrix> *GenerateLinearMomentums(int num_cubes)
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

class WorldHandler {


public:

    vector<IBlock *> iblocks;
    vector<MBlock *> mblocks;
    vector<EBlock *> eblocks;
    vector<ZBlock *> zblocks;

    vector<Block *> blocks;

    WorldHandler(int num_i_blocks_plus, int num_i_blocks_neg, int num_z_blocks, int num_m_blocks, int num_e_blocks_1, int num_e_blocks_1_2) {
        srand((unsigned)time(0)); //NULL???
        AddIBlocks(num_i_blocks_plus, true);
        AddIBlocks(num_i_blocks_neg, false);
        AddMBlocks(num_m_blocks);
        AddZBlocks(num_z_blocks);
        AddEBlocks(num_e_blocks_1, 1);
        AddEBlocks(num_e_blocks_1_2, 0.5f);
    }

    void static GetProperties(int num_blocks, std::vector<Matrix> *&positions, std::vector<Matrix> * &linear_momentums);



    void AddBlocks(int num_blocks, bool state) {

    }

    void AddIBlocks(int num_i_blocks, bool state) {
        std::vector<Matrix> *positions, *linear_momentums;
        GetProperties(num_i_blocks, positions, linear_momentums);
        for(int i=0; i<num_i_blocks; i++) {
            cout << "Add I Block ID: " << i << endl;
            auto new_block = new IBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0)  ,state);
            iblocks.push_back(new_block);
            blocks.push_back(new_block);
            iblocks.at(i)->SetLinearMomentum(linear_momentums->at(i));
        }
    }

    void AddMBlocks(int num_m_blocks) {
        std::vector<Matrix> *positions,  *linear_momentums;
        GetProperties(num_m_blocks, positions, linear_momentums);
        for(int i=0; i<num_m_blocks; i++) {
            cout << "Add M Block ID: " << i << endl;
            auto new_block =  new MBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0));
            mblocks.push_back(new_block);
            blocks.push_back(new_block);
            mblocks.at(i)->SetLinearMomentum(linear_momentums->at(i));
        }
    }

    void AddEBlocks(int num_e_blocks, real k) {
        std::vector<Matrix> *positions, *linear_momentums;
        GetProperties(num_e_blocks, positions, linear_momentums);
        for(int i=0; i<num_e_blocks; i++) {
            cout << "Add E Block ID: " << i << endl;
            auto new_block = new EBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0), k);
            eblocks.push_back(new_block);
            blocks.push_back(new_block);
            eblocks.at(i)->SetLinearMomentum(linear_momentums->at(i));
        }
    }

    void AddZBlocks(int num_z_blocks) {
        std::vector<Matrix> *positions,  *linear_momentums;
        GetProperties(num_z_blocks, positions, linear_momentums);
        for(int i=0; i<num_z_blocks; i++) {
            cout << "Add Z Block ID: " << i << endl;
            auto new_block = new ZBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0));
            zblocks.push_back(new_block);
            blocks.push_back(new_block);
            zblocks.at(i)->SetLinearMomentum(linear_momentums->at(i));
        }
    }

    void Update() {
        for(int i=0; i<this->blocks.size(); i++) {
            this->blocks.at(i)->Update();
        }
    };


    void CollisionHandler() {
        vector<Contact> contact_list;
        for(int i=0; i<blocks.size()-1; i++) {
            for(int j=0; j<blocks.size(); j++) {
                if(i == j) {
                    continue;
                }
                Cube::CollisionDetect(blocks.at(i), blocks.at(j), contact_list);
            }
        }
        for(int i=0; i<contact_list.size(); i++) {
            Cube::CollisionResolution(contact_list.at(i));
        }
        PassBlockFlares(contact_list);
        SpinWorldBlocks();
        IncFlareValuesAndReset();
    }

    void AddForces() {
        for(auto &block: this->blocks) {
            ReactToAllBlocks(block);
        }
    }

    void ReactToAllBlocks(Block *block) {
        for(int i=0; i<iblocks.size(); i++) {
            Matrix to_cube = iblocks.at(i)->position - block->position;
            real squared_dist = Matrix::SquaredNorm(to_cube);
            if(squared_dist >= 25) {
                block->React(iblocks.at(i), squared_dist, to_cube);
            }
        }

        for(int i=0; i<zblocks.size(); i++) {
            Matrix to_cube = zblocks.at(i)->position - block->position;
            real squared_dist = Matrix::SquaredNorm(to_cube);
            if(squared_dist >= 25) {
                block->React(zblocks.at(i), squared_dist, to_cube);
            }
        }

        for(int i=0; i<eblocks.size(); i++) {
            Matrix to_cube = eblocks.at(i)->position - block->position;
            real squared_dist = Matrix::SquaredNorm(to_cube);
            if(squared_dist >= 25) {
                block->React(eblocks.at(i), squared_dist, to_cube);
            }
        }

        for(int i=0; i<mblocks.size(); i++) {
            Matrix to_cube = mblocks.at(i)->position - block->position;
            real squared_dist = Matrix::SquaredNorm(to_cube);
            if(squared_dist >= 25) {
                block->React(mblocks.at(i), squared_dist, to_cube);
            }
        }
    }

    void PassBlockFlares(vector<Contact> &contacts) {
        for(int i=0; i<contacts.size(); i++) {
            PassFlare(contacts.at(i).body1, contacts.at(i).body2);
        }
    }

    void static PassFlare(Block *a, Block *b) {
        real flare_from_a = a->ExtractFlareFromBlock();
        real flare_from_b = b->ExtractFlareFromBlock();

        a->AddFlareToBlock(flare_from_b);
        b->AddFlareToBlock(flare_from_a);
    }

    void IncFlareValuesAndReset() {
        for(int i=0; i<this->blocks.size(); i++) {
            this->blocks.at(i)->UpdateFlare();
            this->blocks.at(i)->flare_inc = 0;
        }
    }

    void AddSpin(vector<Block *> *block_list,  Matrix const &force_direction) {
        for(int i=0; i<block_list->size(); i++) {
            block_list->at(i)->spin(force_direction);
        }
    }

    void SpinWorldBlocks() {
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

};
#endif
