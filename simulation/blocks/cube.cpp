#ifndef MWORLD_SIMULATION_CUBE_H
#define MWORLD_SIMULATION_CUBE_H

#include <iostream>
//#include <stdlib.h>
#include <cstdlib>

#include <cmath>
#include <vector>

#include <matrix.hpp>
#include <quaternion.hpp>

using namespace numerics;
using namespace settings;
using namespace std;

// TODO
// 1) Add Edge to Edge collision pg319 in pdf and pg296 in book
// 3) Make position, velocity, momentum etc as 4x1 instead of 3x1
// 4) Make both FaceToPoint and DetectCubeEdgeEdge collision functions return the min distance between cubes,
//      this way we do not need to use the GJK algorithm to compute min distance

class Cube;

const real right_normal_values[3] = {1, 0, 0};
const real left_normal_values[3] = {-1, 0, 0};
const real up_normal_values[3] = {0, 1, 0};
const real down_normal_values[3] = {0,-1, 0};
const real in_normal_values[3] = {0, 0, -1};
const real out_normal_values[3] = {0, 0, 1};

struct Contact
{
    numerics::Matrix point;     //collision point
    settings::real penetration; //The amount of penetration
    int contact_normal;         // The contact normal // =-1
    Cube *body1;                //The body pointer of the first cube //=nullptr
    Cube *body2;                //The body pointer of the second cube //=nullptr
};

class Cube
{
public:

    const Matrix normals[6] = {
            Matrix(3, 1, left_normal_values), Matrix(3, 1, right_normal_values),
            Matrix(3,1, down_normal_values), Matrix(3, 1, up_normal_values), //please make this static
            Matrix(3, 1, in_normal_values), Matrix(3, 1, out_normal_values)};

    //primary
    Matrix position = Matrix(3.0, 1.0);
    Quaternion orientation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

    Matrix momentum = Matrix(3, 1);
    Matrix angular_momentum = Matrix(3, 1);

    //secondary
    Matrix linear_velocity = Matrix(3, 1);
    Matrix angular_velocity = Matrix(3, 1);

    //constants
    real const inverse_inertia;
    real const inverse_mass;
    real const cube_length;

    real const cube_wrapper_length;



    /**
     * @brief Construct a new Cube object.
     *
     * @param cube_length Length of cube
     * @param position Initial position of the cube.
     * @param initial_orientation Initial orientation of cube
     * @param inverse_mass Inverse mass of cube
     * @param inverse_inertia Inverse inertia of cube
     */
    Cube(real cube_length, Matrix position, Quaternion initial_orientation = Quaternion(1, 0, 0, 0),
         real inverse_mass = 1.0f, real inverse_inertia = 1.0f) : cube_length(cube_length),
                                                                  inverse_mass(inverse_mass),
                                                                  inverse_inertia(inverse_inertia),
                                                                  cube_wrapper_length(cube_length + 0.01)
    {
        this->position = position;
        this->orientation = initial_orientation;
    };

    /**
     * @brief Updates the state of the cube
     *
     */
    void Update()
    {

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

        position += linear_velocity;
        orientation += spin;
        orientation.Normalise();
    }

    /**
     * @brief Returns the equivelent vector of a given vector in cube coordinates
     *
     * @param world_vector
     * @return Matrix
     */
    Matrix ConvertToCubeCoordinates(Matrix const world_vector) const
    {
        return Matrix::MatMul((Quaternion::GetOrientationMatrix3(this->orientation)), world_vector);
    }

    /**
     * @brief Adds a torque to the cube.
     *
     * @param force The force vector in world coordinates
     * @param force_world_cooridinates The coordinates of the force in world coordinates
     * @param dt The amount of time the force was applied for
     */
    void AddTorque(Matrix const force, Matrix const &force_world_cooridinates, real const dt)
    {
        /*
            Initially the force and the force_world_coordinates are in world coordintes. However, the
            angular_momentums (which is a vector quantity) of the cube is in cube_coordinates.
            Hence, we need to first transform the force, and the force_world_coordinates to cube coordinates.

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
        Matrix force_cube_coordinates = force;

        //Matrix r = ConvertToCubeCoordinates(force_world_cooridinates - this->position);
        Matrix r = force_world_cooridinates - this->position;

        // Torque is calculated via Matrix::VectorProduct(force_cube_coordinates, r) * dt

        momentum += force * dt;
        angular_momentum += Matrix::VectorProduct(r, force_cube_coordinates) * dt;

    }

    /**
     * @brief Set the Angular Momentum To Zero
     *
     */
    void SetAngularMomentumToZero()
    {
        angular_momentum = Matrix(3, 1);
    }

    /**
     * @brief Gets the Orientation Matrix of the Cube Object. (Can be used to transform a vector from
     * cube coordinates to world coordinates -? not too sure )
     *
     * @return Matrix&
     */
    Matrix &GetOrientationMatrix() const
    {
        return Quaternion::GetMatrixTransformation(this->orientation);
    }

    /**
     * @brief Get the Inverse Orientation Matrix of the cube Object.
     *
     * @return Matrix&
     */
    Matrix &GetInverseOrientationMatrix() const
    {
        return Quaternion::GetInverseMatrixTransformation(this->orientation);
    }

    Matrix GetVerticesWorldCoordinates() const
    {
        real vertices[24];
        real diff = cube_length / 2;
        for (unsigned int i = 0; i < 8; i++)
        {
            vertices[i + 0] = (i % 2 == 0 ? -diff : diff);
            vertices[i + 8] = (((int)std::floor(i / 2)) % 2 == 0 ? -diff : diff);
            vertices[i + 16] = (((int)std::floor(i / 4)) % 2 == 0 ? -diff : diff);
        }
        Matrix out = Matrix::MatMul(Quaternion::GetInverseOrentationMatrix3(this->orientation), Matrix(3, 8, vertices));
        return out;
    }

    /**
     * @brief Detect collision between Cube A and Cube B
     *
     * @param A
     * @param B
     * @return Contact
     */
    void static CollisionDetect(Cube *a, Cube *b, vector<Contact> &contact_list)
    {
        Matrix *b_world_vertices = b->GetVerticesWorldCoordinates().GetColumns();
        Matrix *vertices = numerics::Matrix::MatMul(Quaternion::GetOrientationMatrix3(a->orientation), b->GetVerticesWorldCoordinates()).GetColumns();

        vector<Matrix> edge_points = {
                vertices[0], vertices[1],
                vertices[0], vertices[2],
                vertices[1], vertices[3],
                vertices[2], vertices[3],
                vertices[6], vertices[7],
                vertices[4], vertices[5],
                vertices[6], vertices[4],
                vertices[7], vertices[5],
                vertices[6], vertices[2],
                vertices[7], vertices[3],
                vertices[0], vertices[4],
                vertices[1], vertices[5]
        };

        for(int i=0; i<8; i++) {
            vertices[i] = vertices[i] + b->position - a->position;
        }

        for(int i=0; i<8; i++)
        {
            if(IsCollision(*a, vertices[i], a->cube_length)) {
                contact_list.push_back(CollisionDetectCubePoint(a, vertices[i], b_world_vertices[i] + b->position, b));
            };
        }


        for (int i = 0; i < edge_points.size(); i += 2) {
            real dot_i_i = Matrix::Dot(edge_points.at(i), edge_points.at(i));
            real dot_i_i_1 = Matrix::Dot(edge_points.at(i), edge_points.at(i + 1));
            if(IsEdgeEdgeCollision(a, edge_points.at(i), edge_points.at(i+1), b, a->cube_length, dot_i_i, dot_i_i_1)) {
                AddCollisionDetectEdgeEdge(a, edge_points.at(i), edge_points.at(i+1), b, contact_list, dot_i_i, dot_i_i_1);
            }
        };

    }

    // make the below two functions private
    /**
     * @brief Returns the contact information of a cube in contact with point
     *
     * @param A
     * @param point
     * @param b
     * @return Contact
     */
    Contact static CollisionDetectCubePoint(Cube *a, Matrix &point, Matrix &world_point, Cube *b)
    {
        return ResolveCollision(a, point, world_point, b);
    }

    /**
     * @brief Detecting Collision between cubes, using edge to edge collision
     *
     * @param edge  The edge is a matrix of size 4x2. Columns represent the points
     * @return Contact
     */
    void static AddCollisionDetectEdgeEdge(Cube *a, Matrix const &edge_p1, Matrix const &edge_p2, Cube *b, vector<Contact> &contact_list, real edge_p1_dot_edge_p2, real edge_p2_dot_edge_p2)
    {

        real lambda =  edge_p1_dot_edge_p2 / edge_p2_dot_edge_p2 * -1;
        Matrix min_point = edge_p1 + edge_p2 * lambda;

        //make resolve collision more faster, since the world coordinate is not always required
        //return ResolveCollision(a, min_point, Matrix::MatMul(a->GetTransformationMatrix(), min_point), b);
        contact_list.push_back(ResolveCollision(a, min_point, Matrix::MatMul(Quaternion::GetInverseOrentationMatrix3(a->orientation), min_point) + a->position, b));
    }

    bool static IsEdgeEdgeCollision(Cube *a, Matrix const &edge_p1, Matrix const &edge_p2, Cube *b, real edge_size, real edge_p1_dot_edge_p2, real edge_p2_dot_edge_p2) {
        // equation of line = p1 + lambda * p2

        real lambda =  edge_p1_dot_edge_p2 / edge_p2_dot_edge_p2 * -1;
        Matrix min_point = edge_p1 + edge_p2 * lambda;

        if (lambda < 0 || lambda > 1 || std::abs(min_point(0, 0)) > edge_size/ 2 ||
            std::abs(min_point(1, 0)) > edge_size / 2 ||
            std::abs(min_point(2, 0)) > edge_size / 2)
        {
            return false;
        }
        return true;
    }


    /**
     * @brief Checks if there is a collision between a particular point and a cube
     *
     * @param a Cube
     * @param cube_point coordinate of point in cube coordinate. Is a 3x1 vector
     * @return bool
     */
    static bool IsCollision(Cube &a, Matrix &cube_point, real edge_size)
    {
        if (std::abs(cube_point(0, 0)) > edge_size/2 || std::abs(cube_point(1, 0)) > edge_size/2 ||
            std::abs(cube_point(2, 0)) > edge_size/2)
        {
            return false;
        };

        return true;
    }

    /**
     * @brief Resolves a collision struct
     *
     * @param a
     * @param cube_point Point in cube coordinate
     * @param contact_info Fills the contact info
     * @param world_point cube point but in world coordinate
     * @return true
     * @return false
     */
    static Contact ResolveCollision(Cube *a, Matrix &cube_point, Matrix &world_point, Cube *b)
    {

        real min_depth = a->cube_length/2 - std::abs(cube_point(0, 0));
        int normal = cube_point(0, 0) < 0 ? 0 : 1;

        real depth = a->cube_length/2 - std::abs(cube_point(1, 0));
        if (depth < min_depth)
        {
            min_depth = depth;
            normal = cube_point(1, 0) < 0 ? 2 : 3;
        }

        depth = a->cube_length /2- std::abs(cube_point(2, 0));
        if (depth < min_depth)
        {

            min_depth = depth;
            normal = cube_point(2, 0) < 0 ? 4 : 5;
        }

        Contact contact_info = {world_point, min_depth, normal, a, b};
        return contact_info;
    }

    /**
     * @brief Get the transformation matrix of cube. Transforms from cube to world coordinaes. Is a 4x4 matrix
     *
     * @return Matrix
     */
    Matrix GetTransformationMatrix() const
    {
        settings::real qw = this->orientation.r;
        settings::real qx = this->orientation.i;
        settings::real qy = this->orientation.j;
        settings::real qz = this->orientation.k;

        settings::real *values = new settings::real[16];
        values[0] = 1.0f - 2.0f * qy * qy - 2.0f * qz * qz;
        values[1] = 2.0f * qx * qy - 2.0f * qz * qw;
        values[2] = 2.0f * qx * qz + 2.0f * qy * qw;
        values[3] = 0.0f;

        values[4] = 2.0f * qx * qy + 2.0f * qz * qw;
        values[5] = 1.0f - 2.0f * qx * qx - 2.0f * qz * qz;
        values[6] = 2.0f * qy * qz - 2.0f * qx * qw;
        values[7] = 0.0f;

        values[8] = 2.0f * qx * qz - 2.0f * qy * qw;
        values[9] = 2.0f * qy * qz + 2.0f * qx * qw;
        values[10] = 1.0f - 2.0f * qx * qx - 2.0f * qy * qy;
        values[11] = 0.0f;

        values[12] = this->position(0, 0);
        values[13] = this->position(1, 0);
        values[14] = this->position(2, 0);
        values[15] = 1.0f;

        Matrix *result = new Matrix(4, 4, values);
        return *result;
    };

    Matrix GetNormal(int normal_id) {
        //return Cube::normals[normal_id];
        return Matrix::MatMul(Quaternion::GetOrientationMatrix3(this->orientation), Cube::normals[normal_id]) ;
    }

    void static CollisionResolution(Contact contact) {
        //numerics::Matrix point = contact.point;     //collision point in world coordinates
        settings::real penetration = contact.penetration; //The amount of penetration
        int contact_normal = contact.contact_normal;         // The contact normal // =-1
        Cube *body1 = contact.body1;                //The body pointer of the first cube //=nullptr
        Cube *body2 = contact.body2;                //The body pointer of the second cube //=nullptr

        Matrix r_ap = contact.point - body1->position;
        Matrix r_bp = contact.point - body2->position;

        Matrix v_ap1 = body1->linear_velocity +  Matrix::VectorProduct(body1->angular_velocity, r_ap);
        Matrix v_bp1 = body2->linear_velocity +  Matrix::VectorProduct(body2->angular_velocity, r_bp);

        Matrix v_ab_1 = body1->linear_velocity + Matrix::VectorProduct(body1->angular_velocity, r_ap) - body2->linear_velocity - Matrix::VectorProduct(body2->angular_velocity, r_bp);

        Matrix normal = body2->GetNormal(contact.contact_normal); //not too sure weather its body1, or body2

        Matrix r_ap__n = Matrix::VectorProduct(r_ap, normal);
        Matrix r_bp__n = Matrix::VectorProduct(r_bp, normal);

        real j = Matrix::Dot(v_ab_1, normal) * -2 / (body1->inverse_mass + body2->inverse_mass + Matrix::Dot(r_ap__n, r_ap__n)*body1->inverse_inertia +
                                                     Matrix::Dot(r_bp__n, r_bp__n)*body2->inverse_inertia);

        Matrix w_a2 = body1->angular_velocity + Matrix::VectorProduct(r_ap, normal * j) * body1->inverse_inertia;
        Matrix w_b2 = body2->angular_velocity - Matrix::VectorProduct(r_bp, normal * j) * body2->inverse_inertia;

        Matrix v_a2 = body1->linear_velocity + normal * j * body1->inverse_mass;
        Matrix v_b2 = body2->linear_velocity - normal * j * body2->inverse_mass;

        body1->momentum = v_a2 / body1->inverse_mass;
        body2->momentum = v_b2 / body2->inverse_mass;

        body1->angular_momentum = w_a2 / body1->inverse_inertia;
        body2->angular_momentum = w_b2 / body2->inverse_inertia;

        body1->position = body1->position - normal * (contact.penetration/2);
        body2->position = body2->position + normal * (contact.penetration/2);


    }
};

#endif
