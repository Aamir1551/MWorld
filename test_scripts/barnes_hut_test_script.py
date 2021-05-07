# python file used to test barnes-hut algorithm
import subprocess

world_size_min_x = "0"
world_size_min_y = "0"
world_size_min_z = "0"

f = open("../results/integration_test_results/barnes_hut_test.txt", "a")

for i in range(10, 110, 10):
    for j in range(100, 500, 100):
        for t in range(0, 15, 1):
            i = str(i)
            j = str(j)
            t = str(t/10)
            result = subprocess.run(["../build/simulation/tree_integration_tests/barnes_hut_test", "10", i, i, i, i, i, i,
                                 world_size_min_x, world_size_min_x+j, world_size_min_y, world_size_min_y + j, world_size_min_z,
                                 world_size_min_z + j, t], stdout=subprocess.PIPE).stdout.decode('utf-8')
            f.write("MSE across frame with (n, world_size, theta) = (" + str(i) + ", " + str(j) + ", " + str(t) + "): ");
            f.write(result)
f.close()