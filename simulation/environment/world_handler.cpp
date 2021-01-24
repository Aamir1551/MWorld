//
// Created by aamir on 24/01/2021.
//

#ifndef MWORLD_SIMULATION_WORLD_HANDLER_H
#define MWORLD_SIMULATION_WORLD_HANDLER_H

#include <cube.cpp>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <vector>

#include <block.cpp>
#include <i_block.cpp>
#include <m_block.cpp>
#include <e_block.cpp>
#include <z_block.cpp>

#include <matrix.hpp>

/*std::vector<Matrix> *GeneratePositions(int num_cubes);
std::vector<Quaternion> *GenerateOrientations(int num_cubes);
std::vector<Matrix> *GenerateAngularMomentums(int num_cubes);
std::vector<Matrix> *GenerateLinearMomentums(int num_cubes);*/

std::vector<Matrix> *GeneratePositions(int num_cubes)
{
    srand((unsigned)time(NULL)); //NULL???
    std::vector<Matrix> *positions = new std::vector<Matrix>;
    float world_size = 500;
    float const scale = world_size / ((float)RAND_MAX / 2.0);
    auto get_coord = [scale, world_size]() -> float { return scale * (rand() - RAND_MAX / 2); };
    for (int i = 0; i < num_cubes; i++)
    {
        real values[] = {get_coord(), get_coord(), get_coord()};
        positions->push_back(Matrix(3, 1, values));
    }
    return positions;
}

std::vector<Matrix> *GenerateAngularMomentums(int num_cubes)
{
    srand((unsigned)time(NULL)); //NULL???
    std::vector<Matrix> *angular_momentums = new std::vector<Matrix>;
    for (int i = 0; i < num_cubes; i++)
    {
        real values[] = {rand() % 5 - 2.0f, rand() % 5 - 2.0f, rand() % 5 - 2.0f};
        angular_momentums->push_back(Matrix(3, 1, values));
    }
    return angular_momentums;
}

std::vector<Matrix> *GenerateLinearMomentums(int num_cubes)
{
    srand((unsigned)time(NULL)); //NULL???
    std::vector<Matrix> *linear_momentums = new std::vector<Matrix>;
    for (int i = 0; i < num_cubes; i++)
    {
        real values[] = {rand() % 5 - 2.0f, rand() % 5 - 2.0f, rand() % 5 - 2.0f};
        linear_momentums->push_back(Matrix(3, 1, values));
    }
    return linear_momentums;
}

std::vector<Quaternion> *GenerateOrientations(int num_cubes)
{
    srand((unsigned)time(NULL)); //NULL???
    std::vector<Quaternion> *orientations = new std::vector<Quaternion>;
    for (int i = 0; i < num_cubes; i++)
    {
        orientations->push_back(Quaternion(1.0f,  rand() % 5 - 2.0f, rand() % 5 - 2.0f, rand() % 5 - 2.0f ));
        orientations->at(i).Normalise();
    }
    return orientations;
}

class WorldHandler {


public:

    vector<Block> blocks;

    WorldHandler(int num_i_blocks_plus, int num_i_blocks_neg, int num_z_blocks, int num_m_blocks_plus, int num_m_blocks_neg, int num_e_blocks_1, int num_e_blocks_1_2) {
        AddIBlocks(num_i_blocks_plus, true);
        AddIBlocks(num_i_blocks_neg, false);
        AddMBlocks(num_m_blocks_plus, true);
        AddMBlocks(num_m_blocks_neg, false);
        AddZBlocks(num_z_blocks);
        AddEBlocks(num_e_blocks_1, 1);
        AddEBlocks(num_e_blocks_1_2, 0.5f);
    }

    void GetProperties(int num_blocks, std::vector<Matrix> *&positions, std::vector<Quaternion> *&orientations, std::vector<Matrix> *&angular_momentums, std::vector<Matrix> * &linear_momentums) {
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
            blocks.push_back(IBlock(positions->at(i), orientations->at(i)  ,state));
            blocks.at(i).SetAngularMomentum(angular_momentums->at(i));
            blocks.at(i).SetLinearMomentum(linear_momentums->at(i));
        }
    }

    void AddMBlocks(int num_m_blocks, bool state) {
        std::vector<Matrix> *positions,  *angular_momentums, *linear_momentums;
        std::vector<Quaternion> *orientations;
        GetProperties(num_m_blocks, positions, orientations, angular_momentums, linear_momentums);
        for(int i=0; i<num_m_blocks; i++) {
            blocks.push_back(MBlock(positions->at(i), orientations->at(i)  ,state));
            blocks.at(i).SetAngularMomentum(angular_momentums->at(i));
            blocks.at(i).SetLinearMomentum(linear_momentums->at(i));
        }
    }

    void AddEBlocks(int num_m_blocks, real k) {
        std::vector<Matrix> *positions,  *angular_momentums, *linear_momentums;
        std::vector<Quaternion> *orientations;
        GetProperties(num_m_blocks, positions, orientations, angular_momentums, linear_momentums);
        for(int i=0; i<num_m_blocks; i++) {
            blocks.push_back(EBlock(positions->at(i), orientations->at(i), k));
            blocks.at(i).SetAngularMomentum(angular_momentums->at(i));
            blocks.at(i).SetLinearMomentum(linear_momentums->at(i));
        }
    }

    void AddZBlocks(int num_m_blocks) {
        std::vector<Matrix> *positions,  *angular_momentums, *linear_momentums;
        std::vector<Quaternion> *orientations;
        GetProperties(num_m_blocks, positions, orientations, angular_momentums, linear_momentums);
        for(int i=0; i<num_m_blocks; i++) {
            blocks.push_back(ZBlock(positions->at(i), orientations->at(i)));
            blocks.at(i).SetAngularMomentum(angular_momentums->at(i));
            blocks.at(i).SetLinearMomentum(linear_momentums->at(i));
        }
    }

    void Update() {
        for(int i=0; i<this->blocks.size(); i++) {
            this->blocks.at(i).Update();
        };
    };


    void CollisionHandler() {
        vector<Contact> contact_list;
        Cube::CollisionDetect(&blocks.at(0), &blocks.at(1), contact_list);

        real min_penetration_value = 10000000000;
        int min_contact_index = 0;
        for (int i = 0; i < contact_list.size(); i++)
        {
            if (contact_list.at(i).penetration < min_penetration_value)
            {
                min_contact_index = i;
            }
        }
        if(contact_list.size() != 0) {
            //cout << "entered" << endl;
            Cube::CollisionResolution(contact_list.at(min_contact_index));
        }
    }


};



#endif