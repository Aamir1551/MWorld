# python file used to test the collision detection with octree algorithm
import subprocess

world_size_min_x = "0"
world_size_min_y = "0"
world_size_min_z = "0"

f = open("../results/integration_test_results/collision_with_octree.txt", "a")

for i in range(10, 110, 10):
    for j in range(100, 500, 100):
            i = str(i)
            j = str(j)
            result = subprocess.run(["../build/simulation/tree_integration_tests/octree_test", "10", i, i, i, i, i, i,
                                     world_size_min_x, world_size_min_x+j, world_size_min_y, world_size_min_y + j, world_size_min_z,
                                     world_size_min_z + j], stdout=subprocess.PIPE).stdout.decode('utf-8')
            f.write("Result with parameters (n, world_size) = (" + str(i) + ", " + str(j) + "): ");
            f.write(result)
f.close()
