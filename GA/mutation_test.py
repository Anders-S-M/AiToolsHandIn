import subprocess
import numpy as np


print('Compiling C program...')
subprocess.run(['make'])


program_path = 'build/ludo'
print('Running test...')


#test mutation rate, constant game and generation and population size
population_size = 20
games = 100
generations = 2000
path_to_save = 'logs/MutationRate/'

#increment mutation rate by 0.5 from 0.1 to 5.0
for mutation_rate in [0, 0.1, 0.5, 1, 1.5, 2]: #np.arange(1, 6, 0.5):
    print('Running test with population mutations: ' + str(mutation_rate))
    args = [program_path, str(population_size), str(games), str(generations), str(mutation_rate), path_to_save]
    subprocess.run(args)