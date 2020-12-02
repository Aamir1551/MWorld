#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cubes_collection_render.hpp>

int CubesCollection::vao;
int CubesCollection::vbo;
int CubesCollection::ebo;
float CubesCollection::cube_length;
unsigned int CubesCollection::shader_id;

float CubesCollection::vertices[24];
glm::mat4 *CubesCollection::view;
glm::mat4 *CubesCollection::project;

unsigned int CubesCollection::indices[] = {
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

void CubesCollection::ApplyUniforms(int model_id)
{
    int model_loc = glGetUniformLocation(CubesCollection::shader_id, "model");
    int view_loc = glGetUniformLocation(CubesCollection::shader_id, "view");
    int proj_loc = glGetUniformLocation(CubesCollection::shader_id, "proj");

    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(*this->models.at(model_id)));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(*CubesCollection::view));
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(*CubesCollection::project));
}