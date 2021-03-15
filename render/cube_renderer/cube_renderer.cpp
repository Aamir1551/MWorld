#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cube_renderer.hpp>
#include <matrix.hpp>

using namespace settings;
namespace render_utils
{
    unsigned int CubeRenderer::vao;
    unsigned int CubeRenderer::vbo;
    unsigned int CubeRenderer::ebo;
    unsigned int CubeRenderer::shader_id;

    real CubeRenderer::vertices[24];
    glm::mat4 *CubeRenderer::view;
    glm::mat4 *CubeRenderer::project;

    Matrix *CubeRenderer::view_mat;
    Matrix *CubeRenderer::project_mat;

    unsigned int CubeRenderer::indices[] = {
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

    void CubeRenderer::ApplyUniforms(Matrix &model_mat)
    {
        int model_loc = glGetUniformLocation(CubeRenderer::shader_id, "model");
        int view_loc = glGetUniformLocation(CubeRenderer::shader_id, "view");
        int proj_loc = glGetUniformLocation(CubeRenderer::shader_id, "proj");

        //glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_mat));
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model_mat.GetValues());
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, CubeRenderer::view_mat->GetValues());
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, CubeRenderer::project_mat->GetValues());
    }

    void CubeRenderer::ApplyUniforms(glm::mat4 &model_mat)
    {
        int model_loc = glGetUniformLocation(CubeRenderer::shader_id, "model");
        int view_loc = glGetUniformLocation(CubeRenderer::shader_id, "view");
        int proj_loc = glGetUniformLocation(CubeRenderer::shader_id, "proj");

        //glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_mat));
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_mat));
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(*CubeRenderer::view));
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(*CubeRenderer::project));
    }

} // namespace render_utils