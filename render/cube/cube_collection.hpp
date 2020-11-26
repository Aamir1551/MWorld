#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cube.hpp>
#include <vector>
#include <matrix.h>
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

    Matrix GetCordinates(){};

    int ApplyUniforms()
    {
        for (int i = 0; i < this->cubes.size(); i++)
        {
            int model_loc = glGetUniformLocation(shader_id, "model"); // "model" will need to be different for every cube
            int view_loc = glGetUniformLocation(shader_id, "view");
            int proj_loc = glGetUniformLocation(shader_id, "proj");

            glUniformMatrix4fv(model_loc, 1, GL_FALSE, this->cubes.at(i)->GetModel().getValues());
            glUniformMatrix4fv(view_loc, 1, GL_FALSE, this->cubes.at(i)->GetView().getValues());
            glUniformMatrix4fv(proj_loc, 1, GL_FALSE, this->cubes.at(i)->GetProject().getValues());
        }
        return 0;
    }

    real *GetCubeVertices()
    {
        real *vertices = new real[this->cubes.size() * 8 * 3];
        for (int i = 0; i < this->cubes.size(); i++)
        {
            real const *const cube_vertices = this->cubes.at(i)->GetVertices();
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

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        real *cubes_vertices = GetCubeVertices();
        int *get_indexes = GetCubeElements();

        //remember the sizeof(cube_vertices) is not 8
        // 24 since 8*3, where 8 is the number of vertices and 3 points each
        //glBufferData(GL_ARRAY_BUFFER, 24, cubes_vertices, GL_STATIC_DRAW);

        //code below commented out - multi line
        //24 * 4 for the 4 bytes
        glBufferData(GL_ARRAY_BUFFER, 24 * 4, cubes_vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0); //try putting this at the end of the function and see what will happen, (if it still works or not)

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * 4, get_indexes, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0); //unbinding vbo --
        //do u also need to unbind gl_element_array_buffer
        return 0;

        /*for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                std::cout << cubes_vertices[j + i * 3] << " ";
            }
            std::cout << std::endl;
        }

        float vertices[] = {
            0.5f, 0.5f, 0.0f,   // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f   // top left
        };

        unsigned int indices[] = {
            // note that we start from 0!
            0, 1, 3, // first Triangle
            1, 2, 3  // second Triangle
        };

        std::cout << sizeof(vertices) << std::endl;
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0); //try putting this at the end of the function and see what will happen, (if it still works or not)

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0); //unbinding vbo --
        //do u also need to unbind gl_element_array_buffer
        return 0;*/
    }
};