#include <vector>

#include <matrix.hpp>
#include <quaternion.hpp>
#include <block.hpp>
#include <cube.hpp>

using namespace numerics;
using namespace settings;
using namespace std;

namespace blocks {

        void Cube::Update() {

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
            this->orientation += spin;
            this->orientation.Normalise();
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
        //Matrix force_cube_coordinates = ConvertToCubeCoordinates(force);
        //Matrix force_direction_cube_coordinates = force_direction;

        //Matrix r = ConvertToCubeCoordinates(force_world_cooridinates - this->position);
        Matrix r = force_position_world_cooridinates - this->position;

        // Torque is calculated via Matrix::VectorProduct(force_cube_coordinates, r) * dt

        //momentum += force_direction * dt;
        angular_momentum += Matrix::VectorProduct(r, force_direction) * dt;
    }

    void Cube::AddLinearForce(Matrix const &force_direction, real dt) {
        momentum += force_direction * dt;
    }

        void Cube::SetAngularMomentumToZero() {
            angular_momentum = Matrix(3, 1);
        }

        Matrix Cube::GetOrientationMatrix() const {
            return Quaternion::GetMatrixTransformation(this->orientation);
        }

        void Cube::CollisionDetect(Block *c1, Block *c2, vector<Contact> &contact_list) {
            Matrix vect_dist = (c1->position - c2->position);
            real dist = Matrix::Norm(vect_dist);
            real length = (c1->cube_length + c2->cube_length) / 2;
            if (dist <= length) {
                Matrix point = (c1->position + c2->position) / 2;
                vect_dist.Normalise();
                Contact contact_info = {point, length - dist, vect_dist, c1, c2};
                contact_list.push_back(contact_info);
            }
        }


        void Cube::CollisionResolution(Contact &contact) {
            Cube *body1 = contact.body1;                //The body pointer of the first cube //=nullptr
            Cube *body2 = contact.body2;            //The body pointer of the second cube //=nullptr

            Matrix r_ap = contact.point - body1->position;
            Matrix r_bp = contact.point - body2->position;

            Matrix v_ap1 = body1->linear_velocity; //+  Matrix::VectorProduct(body1->angular_velocity, r_ap);
            Matrix v_bp1 = body2->linear_velocity; //+  Matrix::VectorProduct(body2->angular_velocity, r_bp);

            Matrix v_ab_1 = body1->linear_velocity + Matrix::VectorProduct(body1->angular_velocity, r_ap) -
                            body2->linear_velocity - Matrix::VectorProduct(body2->angular_velocity, r_bp);

            Matrix normal = contact.normal *
                            -1;//body2->GetNormal(contact.contact_normal); //not too sure weather its body1, or body2

            Matrix r_ap_cross_normal = Matrix::VectorProduct(r_ap, normal);
            Matrix r_bp_cross_normal = Matrix::VectorProduct(r_bp, normal);

            real j = Matrix::Dot(v_ab_1, normal) * -(1 + 0.5) / (body1->inverse_mass + body2->inverse_mass +
                                                                 Matrix::Dot(r_ap_cross_normal, r_ap_cross_normal) *
                                                                 body1->inverse_inertia +
                                                                 Matrix::Dot(r_bp_cross_normal, r_bp_cross_normal) *
                                                                 body2->inverse_inertia);

            //Matrix w_a2 = body1->angular_velocity + Matrix::VectorProduct(r_ap, normal * j) * body1->inverse_inertia;
            //Matrix w_b2 = body2->angular_velocity - Matrix::VectorProduct(r_bp, normal * j) * body2->inverse_inertia;

            Matrix v_a2 = body1->linear_velocity + normal * j * body1->inverse_mass;
            Matrix v_b2 = body2->linear_velocity - normal * j * body2->inverse_mass;

            body1->momentum = v_a2 / body1->inverse_mass;
            body2->momentum = v_b2 / body2->inverse_mass;

            //body1->angular_momentum = w_a2 / body1->inverse_inertia;
            //body2->angular_momentum = w_b2 / body2->inverse_inertia;


            body1->position = body1->position - normal * (contact.penetration / 2);
            body2->position = body2->position + normal * (contact.penetration / 2);


        }
    };