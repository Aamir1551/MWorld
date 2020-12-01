#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
private:
    float lastX = 400, lastY = 300;
    float camera_speed = 20.0f;

    bool firstMouse = false;
    float yaw = -90.0f;
    float pitch = 0.0f;

    glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
    GLFWwindow *window;

    void ProcessControllerInput(float controller_camera_speed, float controller_sensitivity = 0.05f)
    {

        int axescount;
        int buttoncount;
        float const *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axescount);
        char unsigned const *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttoncount);
        if (buttons[4] == 1)
            glfwSetWindowShouldClose(window, true);

        controller_camera_speed += controller_camera_speed * (axes[4] * 2 + 1) * 0.5; //Allow r1 to make it even faster
        camera_pos -= controller_camera_speed * camera_front * axes[1];
        camera_pos += glm::normalize(glm::cross(this->camera_front, this->camera_up)) * controller_camera_speed * axes[0];

        float xoffset = axes[2];
        float yoffset = -axes[3];
        controller_sensitivity += controller_sensitivity * (axes[5] * 2 + 1) * 0.5; //Allow l1 to make it even faster
        xoffset *= controller_sensitivity;
        yoffset *= controller_sensitivity;
        yaw += xoffset;
        pitch += yoffset;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->camera_front = glm::normalize(direction);
    }

    void ProcessKeyboardInput(float keyboard_camera_speed, float keyboard_sensitivity)
    {

        glm::vec3 dx = glm::normalize(glm::cross(this->camera_front, this->camera_up)) * keyboard_camera_speed; // use right hand rule to figure this out;
        glm::vec3 dy = keyboard_camera_speed * this->camera_front;

        if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(this->window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            this->camera_pos += dy;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            this->camera_pos -= dy;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera_pos += dx;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            this->camera_pos -= dx;
    }

public:
    Camera(GLFWwindow *glfw_window, glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f), float camera_speed = 15.0f)
    {
        this->window = glfw_window;
        this->camera_pos = camera_pos;
        this->camera_front = camera_front;
        this->camera_up = camera_up;
    }

    glm::mat4 CalculateView()
    {
        return glm::lookAt(this->camera_pos, this->camera_pos + camera_front, camera_up);
        //camera position, camera target, world space up
    }

    void UpdateCamera(float delta_time)
    {
        if (glfwJoystickPresent(GLFW_JOYSTICK_1))
            ProcessControllerInput(camera_speed * delta_time, 0.05f);
        ProcessKeyboardInput(camera_speed * delta_time, 0.05f);
    }

    void ProcessMouseControlls(GLFWwindow *window, double xpos, double ypos)
    {

        static bool first_mouse;
        if (first_mouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;
        xoffset *= 0.04;
        yoffset *= 0.04;
        yaw += xoffset;
        pitch += yoffset;
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
        this->camera_front = glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
    }

    /*static auto MouseCallbackHandler(Camera camera)
    {

        auto b = [&camera](GLFWwindow *window, double xpos, double ypos) -> void {
            camera.ProcessMouseControlls(window, xpos, ypos);
        };
        return b;
    }*/
};
