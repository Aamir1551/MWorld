#include <cstring>
#include <unordered_set>
#include <set>
#include <iostream>



#include <world_handler.hpp>
#include <settings.hpp>
#include <matrix.hpp>
#include <omp.h>
#include <chrono>


using namespace std;
using namespace numerics;
using namespace settings;
using namespace blocks;

int main(int argc, char *argv[])
{


#if defined(GLFW_ON)
    cout << "GLFW needs to be turned off for testing BarnesHut" << endl;
    exit(0);
#endif

    if(argc < 15) {
       cout << "Insufficient Parameters for barnes_hut_test.cpp file" << endl;
       exit(0);
    }

    Block::theta = atoi(argv[14]);
    real total_num_blocks = atoi(argv[2]) + atoi(argv[3])+ atoi(argv[4])+ atoi(argv[5])+ atoi(argv[6])+ atoi(argv[7])+ atoi(argv[8]);

    WorldHandler world = WorldHandler( atoi(argv[2]),
                                       atoi(argv[3]),
                                       atoi(argv[4]),
                                       atoi(argv[5]),
                                       atoi(argv[6]),
                                       atoi(argv[7]),
                                      atoi(argv[8]),
                                      atoi(argv[9]),
                                       atoi(argv[10]),
                                       atoi(argv[11]),
                                      atoi(argv[12]),
                                      atoi(argv[13]),
                                      4);


    // Initialising time functions
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

    real total_diff = 0;
    while (
duration_cast<milliseconds>(currentFrame - start_time).count() < atoi(argv[1]) * 1000 // Running for 10 seconds only
    )
    {
       currentFrame = high_resolution_clock::now();
       deltaTime = duration_cast<milliseconds>(currentFrame - lastFrame); // Time between current frame and last frame
       lastFrame = currentFrame;

        // Get all forces generated via brute force
        auto contact_list = world.CollisionHandlerBruteForce();

        vector<Matrix> new_brute_moms;
        world.AddForcesViaBruteForce(deltaTime.count() / 1000.0);
        for(auto const i: world.blocks) {
             new_brute_moms.push_back(i->momentum);
        }
        world.AddForcesViaBruteForce(deltaTime.count() / 1000.0 * -1);

        // Get all forces generated via Barnes-Hut algorithm
        vector<Matrix> new_barnes_moms;
        world.AddForcesViaBarnesHut(deltaTime.count() / 1000.0);
        for(auto const i: world.blocks) {
            new_barnes_moms.push_back(i->momentum);
        }
        world.AddForcesViaBarnesHut(deltaTime.count() / 1000.0 * -1);

        // Calculate the MSE of the forces produces by the Barnes-Hut algorithm compared to the forces produces via brue force
        float diff = 0;
        for(unsigned int i=0; i<new_barnes_moms.size(); i++) {
           diff += Matrix::Norm(new_brute_moms.at(i) - new_barnes_moms.at(i));
        }
        total_diff += diff/total_num_blocks;

        world.AddForcesViaBruteForce(deltaTime.count() / 1000.0);
        world.Update(contact_list, deltaTime.count()/1000.0);

        frame_count++;
        total_frame_count++;
        if(duration_cast<milliseconds>(currentFrame - prev_time).count()>= 1000.0) {
            frame_count = 0;
            prev_time = currentFrame;
        }
    }

    cout << "Average Error across frames: " << total_diff / total_frame_count << endl;

    return 0;
}