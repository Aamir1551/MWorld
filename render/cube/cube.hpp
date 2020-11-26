#ifndef CUBE_RE_H
#define CUBE_RE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <math.h>
#include <vector>

class Cube
{

private:
    static float vertices[24];
    static float cube_length;
    static unsigned int shader_id;

public:
    glm::mat4 *model;
    static glm::mat4 *view;
    static glm::mat4 *project;
    static int vao;
    static int vbo;
    static int ebo;

    static unsigned int constexpr indices[36] = {
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
    Cube(){};

    void static inline InitializeCube(float cube_length, unsigned int vao, unsigned int vbo, unsigned int ebo, glm::mat4 *view, glm::mat4 *project)
    {
        float diff = cube_length / 2;
        for (unsigned int i = 0; i < 8; i++)
        {
            Cube::vertices[i * 3] = (i % 2 == 0 ? -diff : diff);
            Cube::vertices[i * 3 + 1] = (((int)std::floor(i / 2)) % 2 == 0 ? -diff : diff);
            Cube::vertices[i * 3 + 2] = (((int)std::floor(i / 4)) % 2 == 0 ? -diff : diff);
        }

        Cube::vao = vao;
        Cube::vbo = vbo;
        Cube::ebo = ebo;

        Cube::view = view;
        Cube::project = project;
    }

    void ApplyUniforms()
    {
        int model_loc = glGetUniformLocation(shader_id, "model");
        int view_loc = glGetUniformLocation(shader_id, "view");
        int proj_loc = glGetUniformLocation(shader_id, "proj");

        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(*this->model));
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(*Cube::view));
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(*Cube::project));
    }

    void static inline AddVerticesToBuffers()
    {
        glBindVertexArray(Cube::vao);

        glBindBuffer(GL_ARRAY_BUFFER, Cube::vbo);
        glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(float), Cube::vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbinding vbo

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Cube::ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(float), Cube::indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
    }
};
#endif