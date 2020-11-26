#include <glm/glm.hpp>
#include <math.h>

#ifndef CUBE_H
#define CUBE_H
class Cube
{

private:
    float x;
    float y;
    float z;
    float vertices[24];
    float cube_length;

public:
    glm::mat4 *model;
    glm::mat4 *view;
    glm::mat4 *project;

    unsigned int indices[36] = {2, 0, 1,
                                2, 3, 1,
                                6, 4, 0,
                                6, 2, 0,
                                3, 1, 5,
                                3, 7, 5,
                                6, 2, 3,
                                6, 7, 3,
                                4, 5, 1,
                                4, 0, 1,
                                6, 4, 5,
                                6, 7, 3}; //what is constexpre???

    //position is the origin of the cube
    Cube(int x, int y, int z, float cube_length, unsigned int shader_id) : x(x), y(y), z(z)
    {
        this->cube_length = cube_length;
        float diff = cube_length / 2;

        for (unsigned int i = 0; i < 8; i++)
        {
            this->vertices[i * 3] = x + (i % 2 ? -diff : diff);
            this->vertices[i * 3 + 1] = y + (((int)std::floor(i / 2)) % 2 ? -diff : diff);
            this->vertices[i * 3 + 2] = z + (((int)std::floor(i / 4)) % 2 ? -diff : diff);
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