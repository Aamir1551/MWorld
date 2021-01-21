#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <vector>

#include <matrix.hpp>
#include <quaternion.hpp>

using namespace numerics;
using namespace settings;
using namespace std;

// TODO
// 1) Add Edge to Edge collision pg319 in pdf and pg296 in book
// 2) FaceToPoint Collision is incorrect. Does not take into account position (translation) of cube
// 3) Make position, velocity, momentum etc as 4x1 instead of 3x1
// 4) Make both FaceToPoint and DetectCubeEdgeEdge collision functions return the min distance between cubes,
//      this way we do not need to use the GJK algirithm to compute min distance

class Cube;

struct Contact
{
    numerics::Matrix point;     //collision point
    settings::real penetration; //The amount of penetration
    int contact_normal;         // The contact normal // =-1
    Cube *body1;                //The body pointer of the first cube //=nullptr
    Cube *body2;                //The body pointer of the second cube //=nullptr
    bool is_contact;            //Boolean representing if the cubes are in contact
};

class Cube
{
public:
    //primary
    Matrix position = Matrix(3.0, 1.0);
    Quaternion orientation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

    Matrix momentum = Matrix(3, 1);
    Matrix angular_momentum = Matrix(3, 1);

    //secondary
    Quaternion spin = Quaternion(0, 0, 0, 0);
    Matrix linear_velocity = Matrix(3, 1);
    Matrix angular_velocity = Matrix(3, 1);

    //constants
    real const inverse_inertia;
    real const inverse_mass;
    real const cube_length;

    /**
     * @brief Construct a new Cube object.
     * 
     * @param cube_length Length of cube
     * @param position Initial position of the cube.
     * @param initial_orientation Initial orientation of cube
     * @param inverse_mass Inverse mass of cube
     * @param inverse_inertia Inverse intertia of cube
     */
    Cube(real cube_length, Matrix position, Quaternion initial_orientation = Quaternion(1, 0, 0, 0),
         real inverse_mass = 1.0f, real inverse_inertia = 1.0f) : cube_length(cube_length),
                                                                  inverse_mass(inverse_mass),
                                                                  inverse_inertia(inverse_inertia)
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
            
            m = Linear Intertia AKA mass
            I = Angular Intertia AKA moment of intertia

            x = Linear position
            θ = Angular position   

            v = p * 1/m
            w = L * 1/I

            dx/dt = v
            dθ/dt = w
        */

        /*
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
     * @brief Returns the equavilent vector of a given vector in cube coordinates
     * 
     * @param world_vector 
     * @return Matrix 
     */
    Matrix ConvertToCubeCoordinates(Matrix const world_vector) const
    {
        real world_coordinates_values[] = {world_vector(0, 0), world_vector(1, 0), world_vector(2, 0), 0};
        Matrix world_coordintes_vector(4, 1, world_coordinates_values);
        Matrix cube_coordinates_vector = Matrix::MatMul((this->GetOrientationMatrix()), world_coordintes_vector);
        real cube_coordinates_values[] = {cube_coordinates_vector(0, 0), cube_coordinates_vector(1, 0), cube_coordinates_vector(2, 0)};
        return Matrix(3, 1, cube_coordinates_values);
    }

    /**
     * @brief Adds a torque to the cube. 
     * 
     * @param force The force vector in world coordinates
     * @param force_world_cooridinates The coordinates of the force in world coordinates
     * @param dt The amount of time the force was applied for
     */
    void AddTorque(Matrix const force, Matrix const force_world_cooridinates, real const dt)
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
        Matrix force_cube_coordinates = ConvertToCubeCoordinates(force);
        Matrix r = ConvertToCubeCoordinates(force_world_cooridinates - this->position);

        // Torque is calculated via Matrix::VectorProduct(force_cube_coordinates, r) * dt

        momentum += force * dt;
        angular_momentum += Matrix::VectorProduct(force_cube_coordinates, r) * dt;
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
        Matrix out = Matrix::MatMul(this->GetInverseOrientationMatrix(), Matrix(3, 8, vertices));
        return out;
    }

    /**
     * @brief Detect collision between Cube A and Cube B
     * 
     * @param A 
     * @param B 
     * @return Contact 
     */
    Contact static CollisionDetect(Cube *a, Cube *b)
    {
        //for each vertex do a collsiondtectcubepoint
        Matrix *vertices = b->GetVerticesWorldCoordinates().GetColumns();
        vector<Contact> c;
        for (int i = 0; i < 8; i++)
        {
            c.push_back(CollisionDetectCubePoint(a, vertices[i], b));
        }

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
            vertices[1], vertices[5]};

        for (int i = 0; i < edge_points.size(); i += 2)
        {
            c.push_back(CollisionDetectEdgeEdge(a, edge_points.at(i), edge_points.at(i + 1), b));
        };
        //choose collision with the lowest value and then return that contact

        if (c.size() == 0)
        {
            // no collision occured
        }

        real min_penetration_value;
        int contact_index;
        for (int i = 0; i < 20; i++)
        {
            if (c.at(i).penetration < min_penetration_value)
            {
                contact_index = i;
            }
        }
        return c.at(contact_index);
    }

    // make the below two functions private
    /**
     * @brief Returns the contact information of a cube in contact with point
     * 
     * @param A 
     * @param point 
     * @return Contact 
     */
    Contact static CollisionDetectCubePoint(Cube *a, Matrix &point, Cube *b)
    {
        Matrix point_cube_coordinate = numerics::Matrix::MatMul(a->GetInverseOrientationMatrix(), point);
        return ResolveCollision(a, point_cube_coordinate, point, b);
    }

    /**
     * @brief Detecting Collision between cubes, using edge to edge collision
     * 
     * @param edge  The edge is a matrix of size 4x2. Columns represent the points
     * @return Contact 
     */
    Contact static CollisionDetectEdgeEdge(Cube *a, Matrix &edge_p1, Matrix &edge_p2, Cube *b)
    {
        Matrix inv_trans = a->GetInverseTransformationMatrix();
        Matrix c1 = Matrix::MatMul(inv_trans, edge_p1);
        Matrix c2 = Matrix::MatMul(inv_trans, edge_p2);
        Matrix p1 = c1;
        Matrix p2 = c2 - c1;

        //equation of line = p1 + lambda * p2

        real lambda = Matrix::Dot(p2, p2) / Matrix::Dot(p1, p2);
        Matrix min_point = p1 + p2 * lambda;

        if (lambda < 0 || lambda > 1 || std::abs(min_point(0, 0)) > a->cube_length / 2 ||
            std::abs(min_point(1, 0)) > a->cube_length / 2 ||
            std::abs(min_point(2, 0)) > a->cube_length / 2)
        {
            // no collision has taken place
        }

        //make resolve collision more faster, since the world coordinate is not always required
        return ResolveCollision(a, min_point, Matrix::MatMul(a->GetTransformationMatrix(), min_point), b);
    }

    /**
     * @brief Returns if in collision or not
     * 
     * @param a 
     * @return true 
     * @return false 
     */
    static bool IsCollision(Cube &a, Matrix &cube_point)
    {
        if (std::abs(cube_point(0, 0)) > a.cube_length || std::abs(cube_point(1, 0)) > a.cube_length ||
            std::abs(cube_point(2, 0)) > a.cube_length)
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

        real min_depth = a->cube_length - std::abs(cube_point(0, 0));
        int normal = cube_point(0, 0) < 0 ? 0 : 1;

        real depth = a->cube_length - std::abs(cube_point(1, 0));
        if (depth < min_depth)
        {
            min_depth = depth;
            normal = cube_point(1, 0) < 0 ? 2 : 3;
        }

        depth = a->cube_length - std::abs(cube_point(2, 0));
        if (depth < min_depth)
        {

            min_depth = depth;
            normal = cube_point(2, 0) < 0 ? 4 : 5;
        }

        /*numerics::Matrix point;  //collision point
        real penetration;        //The amount of penetration
        int contact_normal = -1; // The contact normal
        Cube *body1 = nullptr;   //The body pointer of the first cube
        Cube *body2 = nullptr;   //The body pointer of the second cube
        bool is_contact;         //Boolean representing if the cubes are in contact*/

        //Contact contact_info = {world_point, min_depth, normal, a, b, true};
        Contact contact_info = {world_point, min_depth, normal, a, b, true};
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

    /**
     * @brief Get the inverse transformation matrix of cube. Transforms from world to cube coordinaes. Is a 4x4 matrix
     * @see https://math.stackexchange.com/questions/152462/inverse-of-transformation-matrix
     * 
     * @return Matrix 
     */
    Matrix GetInverseTransformationMatrix() const
    {
        Matrix neg_pinv_v = Matrix::MatMul(Quaternion::GetInverseMatrixTransformation(this->orientation), this->position) * -1;
    }
};