#include <matrix.h>

#ifndef CUBE_H
#define CUBE_H
class Cube
{

private:
    Matrix position{3, 1};
    Matrix vertices{3, 8};
    Matrix orientation{3, 3}; //should be a quaternions???
    Matrix model{4, 4, 1};
    Matrix view{4, 4, 1};
    Matrix project{4, 4, 1};
    int height;

public:
    static unsigned constexpr int indices[36] = {2, 0, 1,
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

    Cube(Matrix position, float height, unsigned int shader_id)
    {
        this->position = position;
        real const *val = this->position.getValues();
        real d = height / 2;
        real x = val[0] - d;
        real y = val[1] - d;
        real z = val[2] - d;
        real *v = new real[24];
        for (unsigned int i = 0; i < 8; i++)
        {
            v[i] = x + (i % 2 ? height : 0);
            v[i + 1] = y + (i % 4 > 1 ? height : 0);
            v[i + 2] = y + (i >= 4 > 0 ? height : 0);
        }
        Matrix initial_vertices(3, 8, v);
        this->vertices = initial_vertices; //add a move operator in matrix
    };

    real const *const get_vertices()
    {
        return this->vertices.getValues();
    }

    void SetModel(Matrix model)
    {
        this->model = model;
    }

    void SetView(Matrix view)
    {
        this->view = view;
    }

    void SetProject(Matrix project)
    {
        this->project = project;
    }
};
#endif