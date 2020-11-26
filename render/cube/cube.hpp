#include <glm/glm.hpp>
#include <math.h>

#ifndef CUBE_H
#define CUBE_H
class Cube
{

private:
    float vertices[24];
    float cube_length;

public:
    glm::mat4 *model;
    glm::mat4 *view;
    glm::mat4 *project;

    unsigned int indices[36] = {
        6, 2, 3,
        6, 7, 3, //top square

        4, 5, 1,
        4, 0, 1, //bottom square

        6, 4, 5,
        6, 7, 5, //front square

        2, 0, 1,
        2, 3, 1, //back square

        7, 5, 1,
        7, 3, 1, //right-side square

        6, 4, 0,
        6, 2, 0 //left-side square
    };

    //position is the origin of the cube
    Cube(float cube_length, unsigned int shader_id, glm::mat4 *model)

    {
        this->model = model;
        this->cube_length = cube_length;
        float diff = cube_length / 2;

        for (unsigned int i = 0; i < 8; i++)
        {
            this->vertices[i * 3] = (i % 2 == 0 ? -diff : diff);
            this->vertices[i * 3 + 1] = (((int)std::floor(i / 2)) % 2 == 0 ? -diff : diff);
            this->vertices[i * 3 + 2] = (((int)std::floor(i / 4)) % 2 == 0 ? -diff : diff);
        }
    };

    float const *const GetVertices()
    {
        return this->vertices;
    }

    void IdentityInitializeCube()
    {
        this->model = new glm::mat4(1);
        this->view = new glm::mat4(1);
        this->project = new glm::mat4(1);
    }
};
#endif