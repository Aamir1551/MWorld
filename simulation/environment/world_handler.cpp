//
// Created by aamir on 24/01/2021.
//

#ifndef MWORLD_SIMULATION_WORLD_HANDLER_H
#define MWORLD_SIMULATION_WORLD_HANDLER_H

#include <cube.cpp>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <ctime>

#include <vector>

#include <block.hpp>
#include <i_block.hpp>
#include <m_block.hpp>
#include <e_block.hpp>
#include <z_block.hpp>

#include <matrix.hpp>

/*std::vector<Matrix> *GeneratePositions(int num_cubes);
std::vector<Quaternion> *GenerateOrientations(int num_cubes);
std::vector<Matrix> *GenerateAngularMomentums(int num_cubes);
std::vector<Matrix> *GenerateLinearMomentums(int num_cubes);*/

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

std::vector<Matrix> *GenerateAngularMomentums(int num_cubes)
{
    auto *angular_momentums = new std::vector<Matrix>;
    auto get_angular_momentums = []() -> real { return (rand() % 1)/100 - 0.0005;};
    for (int i = 0; i < num_cubes; i++)
    {
        real values[] = {get_angular_momentums(), get_angular_momentums(), get_angular_momentums()};
        angular_momentums->push_back(Matrix(3, 1, values));
    }
    return angular_momentums;
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

std::vector<Quaternion> *GenerateOrientations(int num_cubes)
{
    auto *orientations = new std::vector<Quaternion>;
    for (int i = 0; i < num_cubes; i++)
    {
        orientations->push_back(Quaternion(1.0f,  rand() % 5 - 2.0f, rand() % 5 - 2.0f, rand() % 5 - 2.0f ));
        orientations->at(i).Normalise();
    }
    return orientations;
}

class WorldHandler {


public:

    vector<IBlock *> iblocks;
    vector<MBlock *> mblocks;
    vector<EBlock *> eblocks;
    vector<ZBlock *> zblocks;

    vector<Block *> blocks;

    WorldHandler(int num_i_blocks_plus, int num_i_blocks_neg, int num_z_blocks, int num_m_blocks_plus, int num_m_blocks_neg, int num_e_blocks_1, int num_e_blocks_1_2) {
        srand((unsigned)time(0)); //NULL???
        AddIBlocks(num_i_blocks_plus, true);
        AddIBlocks(num_i_blocks_neg, false);
        AddMBlocks(num_m_blocks_plus, true);
        AddMBlocks(num_m_blocks_neg, false);
        AddZBlocks(num_z_blocks);
        AddEBlocks(num_e_blocks_1, 1);
        AddEBlocks(num_e_blocks_1_2, 0.5f);
    }

    void static GetProperties(int num_blocks, std::vector<Matrix> *&positions, std::vector<Quaternion> *&orientations, std::vector<Matrix> *&angular_momentums, std::vector<Matrix> * &linear_momentums) {
        positions = GeneratePositions(num_blocks);
        orientations = GenerateOrientations(num_blocks);
        angular_momentums = GenerateAngularMomentums(num_blocks);
        linear_momentums = GenerateLinearMomentums(num_blocks);
    }

    void AddIBlocks(int num_i_blocks, bool state) {
        std::vector<Matrix> *positions,  *angular_momentums, *linear_momentums;
        std::vector<Quaternion> *orientations;
        GetProperties(num_i_blocks, positions, orientations, angular_momentums, linear_momentums);
        for(int i=0; i<num_i_blocks; i++) {
            cout << "Add I Block ID: " << i << endl;
            auto new_block = new IBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0)  ,state);
            iblocks.push_back(new_block);
            blocks.push_back(new_block);
            //iblocks.at(i)->SetAngularMomentum(angular_momentums->at(i));
            iblocks.at(i)->SetLinearMomentum(linear_momentums->at(i));
        }
    }

    void AddMBlocks(int num_m_blocks, bool state) {
        std::vector<Matrix> *positions,  *angular_momentums, *linear_momentums;
        std::vector<Quaternion> *orientations;
        GetProperties(num_m_blocks, positions, orientations, angular_momentums, linear_momentums);
        for(int i=0; i<num_m_blocks; i++) {
            cout << "Add M Block ID: " << i << endl;
            auto new_block =  new MBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0)  ,state);
            mblocks.push_back(new_block);
            blocks.push_back(new_block);
            //mblocks.at(i)->SetAngularMomentum(angular_momentums->at(i));
            mblocks.at(i)->SetLinearMomentum(linear_momentums->at(i));
        }
    }

    void AddEBlocks(int num_e_blocks, real k) {
        std::vector<Matrix> *positions,  *angular_momentums, *linear_momentums;
        std::vector<Quaternion> *orientations;
        GetProperties(num_e_blocks, positions, orientations, angular_momentums, linear_momentums);
        for(int i=0; i<num_e_blocks; i++) {
            cout << "Add E Block ID: " << i << endl;
            auto new_block = new EBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0), k);
            eblocks.push_back(new_block);
            blocks.push_back(new_block);
            //eblocks.at(i)->SetAngularMomentum(angular_momentums->at(i));
            eblocks.at(i)->SetLinearMomentum(linear_momentums->at(i));
        }
    }

    void AddZBlocks(int num_z_blocks) {
        std::vector<Matrix> *positions,  *angular_momentums, *linear_momentums;
        std::vector<Quaternion> *orientations;
        GetProperties(num_z_blocks, positions, orientations, angular_momentums, linear_momentums);
        for(int i=0; i<num_z_blocks; i++) {
            cout << "Add Z Block ID: " << i << endl;
            auto new_block = new ZBlock(positions->at(i), Quaternion(1.0, 0.0, 0.0, 0.0));
            zblocks.push_back(new_block);
            blocks.push_back(new_block);
            //zblocks.at(i)->SetAngularMomentum(angular_momentums->at(i));
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

    void static PassFlare(Block *a, Block *b) {
        real flare_from_a = a->ExtractFlareFromBlock();
        real flare_from_b = b->ExtractFlareFromBlock();

        a->AddFlareToBlock(flare_from_b);
        b->AddFlareToBlock(flare_from_a);
    }

};



#endif