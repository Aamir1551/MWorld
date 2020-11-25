#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cube.hpp>
#include <vector>
#include <matrix.h>

class ShaderCubeCollection
{
private:
    std::vector<Cube *> cubes;
    unsigned int shader_id;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

public:
    ShaderCubeCollection(unsigned int shader_id)
    {
        this->shader_id = shader_id;
    };

    int add_cube(Cube *cube = 0) // does google style guide allow the use of null pointers???
    {
        if (cube == 0)
        {
            //add random cube
        }
        else
        {
            this->cubes.push_back(cube);
        }
    }

    Matrix GetCordinates(){};

    int ApplyUniforms()
    {
        for (int i = 0; i < this->cubes.size(); i++)
        {
            int model_loc = glGetUniformLocation(shader_id, "model"); // "model" will need to be different for every cube
            int view_loc = glGetUniformLocation(shader_id, "view");
            int proj_loc = glGetUniformLocation(shader_id, "view");

            glUniformMatrix4fv(model_loc, 1, GL_FALSE, this->cubes.at(i)->GetModel().getValues());
            glUniformMatrix4fv(view_loc, 1, GL_FALSE, this->cubes.at(i)->GetView().getValues());
            glUniformMatrix4fv(proj_loc, 1, GL_FALSE, this->cubes.at(i)->GetProject().getValues());
        }
        return 0;
    }

    real *GetCubeVertices()
    {
        real *vertices = new real[this->cubes.size() * 8];
        for (int i = 0; this->cubes.size(); i++)
        {
            real const *const cube_vertices = this->cubes[i]->GetVertices();
            for (int j = 0; j < 8; j++)
            {
                vertices[i * 8 + j] = cube_vertices[j];
            }
        };
        return vertices;
    }

    int *GetCubeElements()
    {
        int *elements = new int[this->cubes.size() * 36];
        for (int i = 0; this->cubes.size(); i++)
        {
            for (int j = 0; j < 36; j++)
            {
                elements[i * 36 + j] = (this->cubes[i]->indices[j] + 36 * i);
            }
        };
        return elements;
    }

    int AddVerticesToBuffers()
    {

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        real *cubes_vertices = GetCubeVertices();
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubes_vertices), cubes_vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0); //try putting this at the end of the function and see what will happen, (if it still works or not)

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        int *getElements = GetCubeElements();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(getElements), getElements, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0); //unbinding vbo --
        //do u also need to unbind gl_element_array_buffer
    }
};