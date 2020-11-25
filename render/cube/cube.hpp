#include <matrix.h>

#ifndef CUBE_H
#define CUBE_H
class Cube
{

private:
    Matrix position{3, 1};
    Matrix vertices{3, 8};    // this is the vertices of the cube
    Matrix orientation{3, 3}; //should be a quaternions???
    Matrix model{4, 4, 1};
    Matrix view{4, 4, 1};
    Matrix project{4, 4, 1};
    int length;

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

    //position is the origin of the cube
    Cube(Matrix position, float length, unsigned int shader_id)
    {
        this->length = length;
        this->position = position;
        real const *val = this->position.getValues();
        real d = length / 2;
        real x = val[0] - d;
        real y = val[1] - d;
        real z = val[2] - d;
        real *v = new real[24];
        for (unsigned int i = 0; i < 8; i++)
        {
            v[i] = x + (i % 2 ? length : 0);
            v[i + 1] = y + (i % 4 > 1 ? length : 0);
            v[i + 2] = y + (i >= 4 > 0 ? length : 0);
        }
        Matrix initial_vertices(3, 8, v);
        this->vertices = initial_vertices; //add a move operator in matrix
    };

    real const *const GetVertices()
    {
        return this->vertices.getValues();
    }

    void SetModel(Matrix model)
    {
        this->model = model;
    }

    Matrix GetModel()
    {
        return this->model;
    }

    void SetView(Matrix view)
    {
        this->view = view;
    }

    Matrix GetView()
    {
        return this->view;
    }

    void SetProject(Matrix project)
    {
        this->project = project;
    }

    Matrix GetProject()
    {
        return this->project;
    }
};
#endif