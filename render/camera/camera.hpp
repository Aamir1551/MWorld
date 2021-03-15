#ifndef MWORLD_RENDER_CAMERA_H
#define MWORLD_RENDER_CAMERA_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <matrix.hpp>
#include <cmath>

using namespace numerics;

namespace render_utils
{

    class Camera
    {

    public:
        Matrix camera_pos_mat = Matrix::CreateColumnVec(0, 0, 3);

    private:
        float camera_speed = 20.0f;

        float yaw = -90.0f;
        float pitch = 0.0f;

        Matrix camera_front_mat = Matrix::CreateColumnVec(0.0f, 0.0f, -1.0f);

        Matrix camera_up_mat = Matrix::CreateColumnVec(0.0f, 1.0f, 0.0f);

        GLFWwindow *window;

        void ProcessControllerInput(float controller_camera_speed, float controller_sensitivity = 0.05f)
        {

            int axescount;
            int buttoncount;
            float const *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axescount);
            char unsigned const *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttoncount);
            if (buttons[4] == 1)
                glfwSetWindowShouldClose(window, true);

            controller_camera_speed += controller_camera_speed * (axes[4] * 2 + 1) * 0.5;

            camera_pos_mat -=  camera_front_mat * controller_camera_speed * axes[1];

            auto temp = (Matrix::VectorProduct(this->camera_front_mat, this->camera_up_mat));
            temp.Normalise();
            camera_pos_mat += temp * controller_camera_speed * axes[0];

            float xoffset = axes[2];
            float yoffset = -axes[3];
            controller_sensitivity += controller_sensitivity * (axes[5] * 2 + 1) * 0.5;
            xoffset *= controller_sensitivity;
            yoffset *= controller_sensitivity;
            yaw += xoffset;
            pitch += yoffset;

            Matrix direction_mat = Matrix::CreateColumnVec(cos(Matrix::ConvertToRadians(yaw)) * cos(Matrix::ConvertToRadians(pitch)), sin(Matrix::ConvertToRadians(pitch)), sin(Matrix::ConvertToRadians(yaw)) * cos(Matrix::ConvertToRadians(pitch)));
            direction_mat.Normalise();
            this->camera_front_mat = direction_mat;

        }

        void ProcessKeyboardInput(float keyboard_camera_speed, float keyboard_sensitivity)
        {


            Matrix dx_mat = Matrix::VectorProduct(this->camera_front_mat, this->camera_up_mat);
            dx_mat.Normalise();
            dx_mat *= keyboard_camera_speed;

            Matrix dy_mat = this->camera_front_mat * keyboard_camera_speed;

            if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(this->window, true);

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                this->camera_pos_mat += dy_mat;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                this->camera_pos_mat -= dy_mat;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                this->camera_pos_mat += dx_mat;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                this->camera_pos_mat -= dx_mat;

        }

    public:
        explicit Camera(GLFWwindow *glfw_window, const Matrix& camera_pos = Matrix::CreateColumnVec(0.0f, 0.0f, 3.0f), const Matrix& camera_front = Matrix::CreateColumnVec(0.0f, 0.0f, -1.0f), const Matrix& camera_up = Matrix::CreateColumnVec(0.0f, 1.0f, 0.0f), float camera_speed = 15.0f)
        {
            this->window = glfw_window;
            this->camera_pos_mat = camera_pos;
            this->camera_front_mat = camera_front;
            this->camera_up_mat = camera_up;
        }

        Matrix CalculateViewMat() {
            return Matrix::LookAt(this->camera_pos_mat, this->camera_pos_mat + this->camera_front_mat, this->camera_up_mat);

        }

        void UpdateCamera(float delta_time)
        {
            if (glfwJoystickPresent(GLFW_JOYSTICK_1))
                ProcessControllerInput(camera_speed * delta_time, 0.05f);
            ProcessKeyboardInput(camera_speed * delta_time, 0.05f);
        }
    };
} // namespace render_utils
#endif