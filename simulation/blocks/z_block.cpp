#include <cmath>

#include <block.hpp>
#include <z_block.hpp>
#include <octree.hpp>
#include <force_octree.hpp>

namespace blocks {

    bool ZBlock::React(ForceOctree *tree, real delta_time) {

        if (tree->zblocks_at_cell_count == 0 && tree->mblocks_at_cell_plus_count == 0) {
            return false;
        }

        Matrix inc_force = Matrix(3, 1);
        bool recurse = ApplyForceFromBlock(tree, delta_time, tree->zblocks_at_cell_count, tree->com_z, inc_force);
        if (recurse == false) {
            recurse = ApplyForceFromBlock(tree, delta_time, tree->mblocks_at_cell_plus_count, tree->com_m_plus,
                                          inc_force);
        }
        if (!recurse) {
            this->AddLinearForce(inc_force, delta_time);
            return false;
        } else {
            return true;
        }
    }

    real ZBlock::ExtractFlareFromBlock(real delta_time) {
        return 0.0;
    };

    void ZBlock::AddFlareToBlock(real flare_amount) {};


}
