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
// 1) Make position, velocity, momentum etc as 4x1 instead of 3x1
// 2) Make both FaceToPoint and DetectCubeEdgeEdge collision functions return the min distance between cubes,
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
    //int contact_normal;         // The contact normal // =-1
    Matrix normal;         // The contact normal // =-1
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
    real const cube_vertices[24];

    Matrix cube_vertices_matrix;
    Matrix *cube_vertices_list;


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
                                                                  cube_vertices{-cube_length/2, cube_length/2, -cube_length/2, cube_length/2, -cube_length/2, cube_length/2, -cube_length/2, cube_length/2,
                                                                                -cube_length/2, -cube_length/2, cube_length/2, cube_length/2, -cube_length/2, -cube_length/2, cube_length/2, cube_length/2,
                                                                                -cube_length/2, -cube_length/2, -cube_length/2, -cube_length/2, cube_length/2, cube_length/2, cube_length/2, cube_length/2}
    {
        this->position = position;
        this->orientation = initial_orientation;
        this->cube_vertices_matrix = Matrix(3, 8, cube_vertices);
        this->cube_vertices_list = this->cube_vertices_matrix.GetColumns();
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

        this->position += linear_velocity;
        this->orientation += spin;
        this->orientation.Normalise();
    }


    /**
     * @brief Adds a torque to the cube.
     *
     * @param force The force vector in world coordinates
     * @param force_world_cooridinates The coordinates of the force in world coordinates
     * @param dt The amount of time the force was applied for
     */
    void AddTorque(Matrix const &force, Matrix const &force_world_cooridinates, real const dt)
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

        /*Matrix force_cube_coordinates = ConvertToCubeCoordinates(force);
        Matrix r = ConvertToCubeCoordinates(force_world_cooridinates - this->position);

        // Torque is calculated via Matrix::VectorProduct(force_cube_coordinates, r) * dt

        momentum += force * dt;
        angular_momentum += Matrix::VectorProduct(force_cube_coordinates, r) * dt;*/


        //Matrix force_cube_coordinates = ConvertToCubeCoordinates(force);
        //Matrix force_cube_coordinates = force;

        //Matrix r = ConvertToCubeCoordinates(force_world_cooridinates - this->position);
        Matrix r = force_world_cooridinates - this->position;

        // Torque is calculated via Matrix::VectorProduct(force_cube_coordinates, r) * dt

        momentum += force * dt;
        angular_momentum += Matrix::VectorProduct(r, force) * dt;

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

        auto *values = new settings::real[16];
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
        delete[] values;
        return *result;
    };

    Matrix GetNormal(int normal_id) {
        //return Cube::normals[normal_id];
        return Matrix::MatMul(Quaternion::GetOrientationMatrix3(this->orientation), Cube::normals[normal_id]);
    }

    static void CollisionDetect(Cube *c1, Cube *c2, vector<Contact> &contact_list) {
        Matrix vect_dist =  (c1->position - c2->position);
        real dist = Matrix::Norm(vect_dist);
        real length = (c1->cube_length + c2->cube_length)/2;
        if(dist < length) {
            cout << "in here" << endl;
            Matrix point = (c1->position + c2->position)/2;
            vect_dist.Normalise();
            Contact contact_info = {point, length - dist, vect_dist, c1, c2};
            contact_list.push_back(contact_info);
        }
    }


    void static CollisionResolution(Contact &contact) {
        cout << "happening";
        Cube *body1 = contact.body1;                //The body pointer of the first cube //=nullptr
        Cube *body2 = contact.body2;            //The body pointer of the second cube //=nullptr

        //Matrix r_ap = body1->position - contact.normal; //contact.point - body1->position;
        //Matrix r_bp = body2->position + contact.normal; //contact.point - body2->position;

        Matrix r_ap = contact.point - body1->position;
        Matrix r_bp = contact.point - body2->position;


        Matrix v_ap1 = body1->linear_velocity +  Matrix::VectorProduct(body1->angular_velocity, r_ap);
        Matrix v_bp1 = body2->linear_velocity +  Matrix::VectorProduct(body2->angular_velocity, r_bp);

        Matrix v_ab_1 = body1->linear_velocity + Matrix::VectorProduct(body1->angular_velocity, r_ap) - body2->linear_velocity - Matrix::VectorProduct(body2->angular_velocity, r_bp);

        Matrix normal = contact.normal * -1;//body2->GetNormal(contact.contact_normal); //not too sure weather its body1, or body2

        Matrix r_ap_cross_normal = Matrix::VectorProduct(r_ap, normal);
        Matrix r_bp_cross_normal = Matrix::VectorProduct(r_bp, normal);

        real j = Matrix::Dot(v_ab_1, normal) * -2 / (body1->inverse_mass + body2->inverse_mass + Matrix::Dot(r_ap_cross_normal, r_ap_cross_normal)*body1->inverse_inertia +
                                                     Matrix::Dot(r_bp_cross_normal, r_bp_cross_normal)*body2->inverse_inertia);

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


