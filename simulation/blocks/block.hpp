#ifndef MWORLD_SIMULATION_BLOCK_H
#define MWORLD_SIMULATION_BLOCK_H

#include <cube.hpp>

namespace blocks {

    class IBlock;
    class EBlock;
    class MBlock;
    class ZBlock;
    class Cube;
    class Octree;
    class CollisionOctree;
    class ForceOctree;

    class Block : public Cube {

    public:

        int const block_id = 0; // block_id is useful for allowing us to make extra optimisations

        // Flare value of cube
        real flare_value = 0.0f;
        // Flare value to increment to total flare value
        real flare_inc = 0.0f;

        real static flare_capacity; // The maximum flare capacity of a cube
        real static theta; // threshold requirement for Barnes-Hut algorithm
        static int block_count; // The number of blocks currently created

        bool locked; // When a block is locked, no forces act upon it

        int block_type = -1; // Block type can be either of [0=IBlock, 1=ZBlock, 2=MBlock, 3=EBlock]

        Block(const Matrix& position, const Quaternion& initial_orientation, real initial_flare_amount, real cube_length = 4.0f) : Cube(cube_length, position,
                                                                                                 initial_orientation,
                                                                                                 1.0f,
                                                                                                 1.0f), block_id(block_count) {
            this->flare_value = initial_flare_amount;
            this->locked = false;
            Block::block_count +=1;
        };

        // Applies a force to a given block
        bool ApplyForceFromBlock(ForceOctree* tree, int cell_count, const Matrix &com, Matrix &inc_force);

        void SetLinearMomentum(Matrix &linear_momentum);
        void SetAngularMomentum(Matrix &angular_momentum);

        // Applies Barnes-Hut algorithm to block from tree
        virtual bool ReactBarnesHut(ForceOctree * tree, real delta_time) = 0;

        // Methods use to calculate the force the block experiances due to blocks of other types
        virtual void ReactSerial(IBlock *b, real delta_time);
        virtual void ReactSerial(MBlock *b, real delta_time);
        virtual void ReactSerial(ZBlock *b, real delta_time);
        virtual void ReactSerial(EBlock *b, real delta_time);

        virtual real ExtractFlareFromBlock(real deltatime) = 0;

        virtual void AddFlareToBlock(real flare_amount, Block *b) = 0;

        virtual void spin(Matrix const &force_direction);

        virtual void UpdateFlare();

        virtual void Decay(real delta_time);
        ~Block() override = default;

    };

}
#endif