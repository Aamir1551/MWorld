#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cube.hpp>
#include <vector>
#include <iostream>

class ShaderCubeCollection
{
private:
    std::vector<Cube *> cubes;
    unsigned int shader_id;
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

public:
    ShaderCubeCollection(unsigned int shader_id, unsigned int &vao, unsigned int &vbo, unsigned int &ebo)
    {
        this->shader_id = shader_id;
        this->vao = vao;
        this->vbo = vbo;
        this->ebo = ebo;
    };

    int add_cube(Cube *cube) // does google style guide allow the use of null pointers???
    {
        this->cubes.push_back(cube);
        return 0;
    }

    int ApplyUniforms()
    {
        for (int i = 0; i < this->cubes.size(); i++)
        {
            int model_loc = glGetUniformLocation(shader_id, "model"); // "model" will need to be different for every cube
            int view_loc = glGetUniformLocation(shader_id, "view");
            int proj_loc = glGetUniformLocation(shader_id, "proj");

            Cube *current_cube = this->cubes.at(i);
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(*current_cube->model));
            glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(*current_cube->view));
            glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(*current_cube->project));
        }
        return 0;
    }

    float *GetCubeVertices()
    {
        float *vertices = new float[this->cubes.size() * 8 * 3];
        for (int i = 0; i < this->cubes.size(); i++)
        {
            float const *const cube_vertices = this->cubes.at(i)->GetVertices();
            for (int j = 0; j < 8; j++)
            {
                vertices[0 + i * 24 + j] = cube_vertices[j];
                vertices[8 + i * 24 + j] = cube_vertices[j + 8];
                vertices[16 + i * 24 + j] = cube_vertices[j + 16];
            }
        };
        return vertices;
    }

    int *GetCubeElements()
    {
        int *elements = new int[this->cubes.size() * 36];

        for (int i = 0; i < this->cubes.size(); i++)
        {
            for (int j = 0; j < 36; j++)
            {
                elements[i * 36 + j] = (this->cubes.at(i)->indices[j] + 36 * i);
            }
        };
        return elements;
    }

    int AddVerticesToBuffers()
    {

        float *cubes_vertices = GetCubeVertices();
        int *get_indexes = GetCubeElements();

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(float), cubes_vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbinding vbo

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(float), get_indexes, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        return 0;
    }
};