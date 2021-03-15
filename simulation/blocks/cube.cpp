#include <vector>
#include <omp.h>

#include <matrix.hpp>
#include <quaternion.hpp>
#include <block.hpp>
#include <cube.hpp>

using namespace numerics;
using namespace settings;
using namespace std;

namespace blocks {

        void Cube::Update(real min_x, real max_x, real min_y, real max_y, real min_z, real max_z, real dt) {

            /*
            Equations of motion being used are:
                p = Linear momentum
                L = Angular momentum

                v = Linear velocity
                w = Angular velocity

                m = Linear Inertia AKA mass
                I = Angular Inertia AKA moment of inertia

                x = Linear position
                θ = Angular position

                v = p * 1/m
                w = L * 1/I

                dx/dt = v
                dθ/dt = w

            Updating orientation quaternion involves the steps:
                1)  Convert Scaled axes representation of angular velocity to a quaternion
                    scaled axes quaternion = (angular_velocity[0], angular_velocity[1], angular_velocity[2])
                2)  Use the equation:
                            new orientation = old orientation + 1/2 * scaled axes quaternion * old orientation

            */

            linear_velocity = momentum * inverse_mass;
            angular_velocity = angular_momentum * inverse_inertia;

            Quaternion q(0, angular_velocity(0, 0), angular_velocity(1, 0), angular_velocity(2, 0));
            Quaternion spin = q * orientation;

            this->position += linear_velocity;
            this->position.ApplyMinVector3(max_x, max_y, max_z);
            this->position.ApplyMaxVector3(min_x, min_y, min_z);
            this->orientation += spin;
            this->orientation.Normalise();
            Cube::CollisionBoundary(this, min_x, max_x, min_y, max_y, min_z, max_z);
        }


    /**
     * @brief Adds a torque to the cube.
     *
     * @param force The force direction
     * @param force_world_cooridinates The force position in world coordinates
     * @param dt The amount of time the force was applied for
     */
    void Cube::AddTorque(Matrix const &force_direction, Matrix const &force_position_world_cooridinates, real const dt)
    {
        /*
            Initially the force and the force_world_coordinates are in world coordintes. However, the
            angular_momentums (which is a vector quantity) of the cube is in cube_coordinates.
            Hence, we need to first transform the force, and the force_world_coordinates to cube coordinates.
        */

        /*
        Equations of motion being used are:
            p = Linear momentum
            L = Angular momentum

            torque = perpendicular distance form pivot * force

            linear impulse (Δp) = force * time
            Angular impulse (ΔL) = torque * time

            dp/dt = Δp (Rate of change of linear momentum)
            dL/dt = ΔL (Rate of change of angular momentum)

            New linear momentum = old linear momentum + linear impulse
            New angular momentum = old angular momentum + angular impulse

       */
        Matrix r = force_position_world_cooridinates - this->position;
        angular_momentum += Matrix::VectorProduct(r, force_direction) * dt;
    }

    void Cube::AddLinearForce(Matrix const &force_direction, real dt) {
        momentum += force_direction * dt;
        momentum -= force_direction * dt; //comment out this line of code to test out time complexity of code, when forces are not being take in to consideration
    }

    void Cube::SetAngularMomentumToZero() {
        angular_momentum = Matrix(3, 1);
    }

    void Cube::SetLinearMomentumToZero() {
        momentum = Matrix(3, 1);
    }


    Matrix Cube::GetOrientationMatrix() const {
        return Quaternion::GetMatrixTransformation(this->orientation);
    }

    void Cube::CollisionDetect(Block *c1, Block *c2, vector<Contact> &contact_list) {
        Matrix vect_dist = (c1->position - c2->position);
        real dist = Matrix::Norm(vect_dist);
        real length = (c1->cube_length + c2->cube_length) / 2;

        // Below line of code needs to be uncommented, but it makes things slower, if we
        // uncomment it. Maybe it has to do with the computer
        // If the below code is commented, the forces will not appear to be working properly
            if(dist == 0) {
                auto temp = c1->position(0, 0);
#pragma omp critical
                {
                    c1->position(0, 0, temp + length/2);
                    c2->position(0, 0, temp - length/2);
                };
                return;
            }

        if (dist <= length) {
            Matrix point = (c1->position + c2->position) / 2;
            vect_dist.Normalise();
            Contact contact_info = {point, length - dist, vect_dist, c1, c2};
#pragma omp critical
            {
                contact_list.push_back(contact_info);
            };
        }
    }

    void Cube::CollisionDetectAndResolve(Block *c1, Block *c2, vector<omp_lock_t> partial_locks) {
        Matrix vect_dist = (c1->position - c2->position);
        real dist = Matrix::Norm(vect_dist);
        real length = (c1->cube_length + c2->cube_length) / 2;
        if(dist == 0) {
            auto temp = c1->position(0, 0);
            c1->position(0, 0, temp + length/2);
            c2->position(0, 0, temp - length/2);
            return;
        }

        if (dist <= length) {
            Matrix point = (c1->position + c2->position) / 2;
            vect_dist.Normalise();
            Contact contact_info = {point, length - dist, vect_dist, c1, c2};


            //int a = max(c1->block_id, c2->block_id);
            //int b = min(c1->block_id, c2->block_id);

            //omp_set_lock(min(&partial_locks[c1->block_id], &partial_locks[c2->block_id]));
            //omp_set_lock(max(&partial_locks[c1->block_id], &partial_locks[c2->block_id]));
            //cout << "came here" << endl;
#if defined(OPENMP)
            omp_set_lock(&partial_locks[min(c1->block_id, c2->block_id)]);
            omp_set_lock(&partial_locks[max(c1->block_id, c2->block_id)]);
#endif
#pragma omp critical (OUTER)
            {
                CollisionResolution(contact_info);
            }

#if defined(OPENMP)
            omp_unset_lock(&partial_locks[min(c1->block_id, c2->block_id)]);
            omp_unset_lock(&partial_locks[max(c1->block_id, c2->block_id)]);
#endif

        }
    }


    void Cube::CollisionResolution(Contact &contact) {
        Cube *body1 = contact.body1;
        Cube *body2 = contact.body2;
        auto normal = contact.normal;
        auto temp = body1->momentum;
#pragma omp critical (INNER)
        {
            body1->momentum = body2->momentum * 0.7;
            body2->momentum = temp * 0.7;
            body1->position += normal * (contact.penetration/2);
            body2->position -= normal * (contact.penetration/2);
        };
    }

    void Cube::CollisionBoundary(Cube *c1, real min_boundary_x, real max_boundary_x, real min_boundary_y,
                                       real max_boundary_y, real min_boundary_z, real max_boundary_z) {
        real x = c1->position(0, 0);
        real y = c1->position(1, 0);
        real z = c1->position(2, 0);
        real mx = c1->momentum(0, 0) * 0.8;
        real my = c1->momentum(1, 0) * 0.8;
        real mz = c1->momentum(2, 0) * 0.8;
        if(x <= min_boundary_x) {
            c1->momentum(0, 0, abs(mx));
        } else if(x >= max_boundary_x) {
            c1->momentum(0, 0, -abs(mx));
        }
        if(y <= min_boundary_y) {
            c1->momentum(1, 0, abs(my));
        } else if(y >= max_boundary_y) {
            c1->momentum(1, 0, -abs(my));
        }
        if(z <= min_boundary_z) {
            c1->momentum(2, 0, abs(mz));
        } else if(z >= max_boundary_z) {
            c1->momentum(2, 0, -abs(mz));
        }
    }
};