//#include <cube.hpp>
#include "./cube.hpp"

int Cube::vao;
int Cube::vbo;
int Cube::ebo;
float Cube::cube_length;
unsigned int Cube::shader_id;

float Cube::vertices[24];
glm::mat4 *Cube::view;
glm::mat4 *Cube::project;