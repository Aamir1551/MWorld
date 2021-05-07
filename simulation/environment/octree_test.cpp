#include <cstring>
#include <unordered_set>
#include <set>

#if defined(GLFW_ON)
    #include <glad/glad.h>
    #include <GLFW/glfw3.h>
#endif

#if defined(GLFW_ON)
    #include <world_initializer.hpp>
    #include <camera.hpp>
    #include <block_renderer.hpp>
#endif

#include <world_handler.hpp>
#include <settings.hpp>
#include <matrix.hpp>
#include <omp.h>
#include <chrono>

using namespace std;


#if defined(GLFW_ON)
    using namespace render_utils;
#endif

using namespace numerics;
using namespace settings;
using namespace blocks;

int main(int argc, char *argv[])
{


#if defined(GLFW_ON)
    WorldProperties *world_properties = WorldIntializer();
    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
#endif


#if defined(GLFW_ON)
    real cube_length = 4.0f;
    Camera camera = Camera(world_properties->window);
    camera.camera_pos_mat = Matrix::CreateColumnVec(0, 0, 250);
    BlockRenderer::InitialiseBlockRenderer(&camera, cube_length, vao, vbo, ebo, world_properties);
#endif

    int num_blocks_same = 100;
    WorldHandler world = WorldHandler(num_blocks_same, num_blocks_same, num_blocks_same, num_blocks_same, num_blocks_same, num_blocks_same, -100, 100, -100, 100, -100 ,100);

#if defined(GLFW_ON)
    glBindVertexArray(vao);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUseProgram(world_properties->shader_id);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //using black to clear the background
#endif
    std::cout << "Entering Main Loop" << std::endl;

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    auto start_time = high_resolution_clock::now();
    auto currentFrame = high_resolution_clock::now();
    auto lastFrame = high_resolution_clock::now(); // Time of last frame
    auto deltaTime = duration_cast<milliseconds>(currentFrame - lastFrame); // time between current frame and last frame

    real frame_count = 0;
    real total_frame_count = 0;

    auto prev_time =  high_resolution_clock::now();


    while (
#if defined(GLFW_ON)
         !glfwWindowShouldClose(world_properties->window)
#else
            duration_cast<milliseconds>(currentFrame - start_time).count() < atoi(argv[1]) * 1000 // time between current frame and last frame
#endif
    )
    {
       currentFrame = high_resolution_clock::now();
       deltaTime = duration_cast<milliseconds>(currentFrame - lastFrame); // Time between current frame and last frame
       lastFrame = currentFrame;

#if defined(GLFW_ON)
        camera.UpdateCamera(deltaTime.count() /1000.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif

        auto contact_list = world.CollisionHandlerBruteForce();
        auto contact_list1 = world.CollisionHandlerWithOctree();

        set<pair<Block *, Block *>> c;
        for(auto const &i : contact_list) {
           c.insert(make_pair(i.body1, i.body2));
           c.insert(make_pair(i.body2, i.body1));
        }

        set<pair<Block *, Block *>> c1;
        for(auto const &i : contact_list1) {
            c1.insert(make_pair(i.body1, i.body2));
            c1.insert(make_pair(i.body2, i.body1));
        }

        if(c != c1 || contact_list.size() != contact_list1.size()) {
            cout << c.size() << " " << c1.size() <<  endl;
            cout << "Not Equal" << endl;
        }

        world.AddForcesViaBarnesHut(deltaTime.count() / 1000.0);
        world.Update(contact_list, deltaTime.count()/1000.0);

#if defined(GLFW_ON)
        BlockRenderer::DrawAllBlocks(&world.iblocks, &world.zblocks, &world.eblocks, &world.mblocks);
        glfwSwapBuffers(world_properties->window);
        glfwPollEvents();
#endif
        frame_count++;
        total_frame_count++;
        if(duration_cast<milliseconds>(currentFrame - prev_time).count()>= 1000.0) {
            frame_count = 0;
            prev_time = currentFrame;
        }
    }


    cout << "Terminating..." << endl;


#if defined(GLFW_ON)
    glDeleteBuffers(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glfwTerminate();
#endif

    cout << "Terminated" << endl;
    return 0;
}