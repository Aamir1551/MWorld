#include <cube.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

int Cube::vao;
int Cube::vbo;
int Cube::ebo;
float Cube::cube_length;
unsigned int Cube::shader_id;

float Cube::vertices[24];
glm::mat4 *Cube::view;
glm::mat4 *Cube::project;

unsigned int Cube::indices[] = {
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

void Cube::ApplyUniforms()
{
    int model_loc = glGetUniformLocation(Cube::shader_id, "model");
    int view_loc = glGetUniformLocation(Cube::shader_id, "view");
    int proj_loc = glGetUniformLocation(Cube::shader_id, "proj");

    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(*this->model));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(*Cube::view));
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(*Cube::project));
}