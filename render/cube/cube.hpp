#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <math.h>
#include <vector>

class Cube
{

public:
    static float cube_length;
    glm::mat4 *model;
    static glm::mat4 *view;
    static glm::mat4 *project;
    static int vao;
    static int vbo;
    static int ebo;
    static float vertices[24];
    static unsigned int shader_id;

    static unsigned int indices[36];

    //position is the origin of the cube
    Cube(){};

    void static InitializeCube(float cube_length, unsigned int vao, unsigned int vbo, unsigned int ebo, glm::mat4 *view, glm::mat4 *project, unsigned int shader_id)
    {
        float diff = cube_length / 2;
        for (unsigned int i = 0; i < 8; i++)
        {
            Cube::vertices[i * 3] = (i % 2 == 0 ? -diff : diff);
            Cube::vertices[i * 3 + 1] = (((int)std::floor(i / 2)) % 2 == 0 ? -diff : diff);
            Cube::vertices[i * 3 + 2] = (((int)std::floor(i / 4)) % 2 == 0 ? -diff : diff);
        }

        Cube::shader_id = shader_id;
        Cube::vao = vao;
        Cube::vbo = vbo;
        Cube::ebo = ebo;

        Cube::view = view;
        Cube::project = project;
    }

    void ApplyUniforms();

    void static AddVerticesToBuffers()
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