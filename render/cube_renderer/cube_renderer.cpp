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

        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model_mat.GetValues());
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, CubeRenderer::view_mat->GetValues());
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, CubeRenderer::project_mat->GetValues());
    }

} // namespace render_utils