#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstdlib> //for the rand function

#include <math.h>
#include "./shader.hpp"
#include <cube.hpp>
#include <vector>
#include <time.h>

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
std::vector<glm::vec3> *GeneratePosition(int num_cubes);
std::vector<glm::vec3> *GenerateRotationsAxis(int num_cubes);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;         // Time between current frame and last frame
float lastFrame = 0.0f;         // Time of last frame
float lastX = 400, lastY = 300; //needed for mouse initial values

bool firstMouse = false;
float yaw = -90.0f;
float pitch = 0.0f;
// TODO:
// 1) Go over mouse controls and understand properly how they work, also how cameraFront works
// so read again from page 97
// 2) Organise code layout properly, and sort out cmakelists files for the full render directory
// 3) Please get notion
// 4) read over random numbers in c++
// 5) statics and linking and why static variables should be in header file only, and why u'll get linking errors if you don't
// 6) Learn to add a colour using element buffer objects
int main()
{
    std::cout << "Rendering started" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    std::cout << "GLFW initialised" << std::endl;

    GLFWwindow *window = glfwCreateWindow(800, 600, "Cube Renderer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create a GLFW window" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    std::cout << "GLFW successfully intialised" << std::endl;

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    std::cout << "GLAD successfully intialised" << std::endl;

    glViewport(0, 0, 800, 600);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader shader("../shaders/vert.glsl", "../shaders/frag.glsl");
    shader.use();
    std::cout << "Shaders successfully initialised" << std::endl;

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao); // this only gives a number to the vao varialbe, doesnt acc create a buffer
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glm::mat4 id = glm::mat4(1.0f);

    glm::mat4 view;
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //camera position, camera target, world space up

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 400.0f);

    int num_cubes = 300;
    std::vector<glm::vec3> *positions = GeneratePosition(num_cubes);
    std::vector<glm::vec3> *rotations = GenerateRotationsAxis(num_cubes);

    Cubes::InitializeCube(1.0f, vao, vbo, ebo, &view, &projection, shader.shader_id);

    Cubes::AddVerticesToBuffers();

    Cubes cubes;

    glBindVertexArray(vao);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUseProgram(shader.shader_id);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //using black
    std::cout << "Entering Main Loop" << std::endl;
    while (!glfwWindowShouldClose(window))
    { // render loop -- an iteration of this main render loop is called a frame
        processInput(window);
        glfwSetCursorPosCallback(window, mouse_callback);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < num_cubes; i++)
        {
            glm::mat4 model = glm::translate(id, positions->at(i));
            model = glm::rotate(model, glm::radians((float)glfwGetTime() * 20 * ((i + 1) % 20)), rotations->at(i));
            cubes.models.push_back(&model);

            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

            cubes.ApplyUniforms(i);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        cubes.models.clear();

        glfwSwapBuffers(window);
        glfwPollEvents(); //checks if any events are triggered, and calls their respective handlers
    }

    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    float cameraSpeed = 10.0f * deltaTime;              //change 2.5 if you want it to move at a different speed
    int present = glfwJoystickPresent(GLFW_JOYSTICK_1); //get player 1 inputs
    if (present)
    {
        int axescount;
        int buttoncount;
        float const *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axescount);
        char unsigned const *b = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttoncount);
        if (b[4] == 1)
        {
            glfwSetWindowShouldClose(window, true);
        }
        cameraSpeed += cameraSpeed * (axes[5] * 2 + 1) * 0.5; //Allow r1 to make it even faster
        cameraPos -= cameraSpeed * cameraFront * axes[1];
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
                     cameraSpeed * axes[0]; // use right hand rule to figure this out

        float xoffset = axes[2];
        float yoffset = -axes[3];
        float sensitivity = 0.03f;
        sensitivity += sensitivity * (axes[4] * 2 + 1) * 0.5; //Allow l1 to make it even faster
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        yaw += xoffset;
        pitch += yoffset;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
                     cameraSpeed; // use right hand rule to figure this out
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
                     cameraSpeed;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    float sensitivity = 0.03f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

std::vector<glm::vec3> *GeneratePosition(int num_cubes)
{
    srand((unsigned)time(NULL)); //NULL???
    std::vector<glm::vec3> *positions = new std::vector<glm::vec3>;
    float world_size = 100;
    float const scale = world_size / ((float)RAND_MAX / 2.0);
    auto get_coord = [scale, world_size]() -> float { return scale * (rand() - RAND_MAX / 2); };
    for (int i = 0; i < num_cubes; i++)
    {
        positions->push_back(glm::vec3(get_coord(), get_coord(), get_coord())); //try this without a pointer to see if it'l work.
    }
    return positions;
}

std::vector<glm::vec3> *GenerateRotationsAxis(int num_cubes)
{
    srand((unsigned)time(NULL)); //NULL???
    std::vector<glm::vec3> *rotations = new std::vector<glm::vec3>;
    for (int i = 0; i < num_cubes; i++)
    {
        rotations->push_back(glm::vec3(rand() % 5 - 2, rand() % 5 - 2, rand() % 5 - 2)); //try this without a pointer to see if it'l work.
    }
    return rotations;
}
