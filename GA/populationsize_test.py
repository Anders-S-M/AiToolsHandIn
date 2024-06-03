import subprocess



print('Compiling C program...')
subprocess.run(['make'])


program_path = 'build/ludo'
print('Running test...')


#test population size, constant game and generation and mutation rate
mutation_rate = 1.0
games = 100
generations = 2000
path_to_save = 'logs/PopulationSize/'

#increment population size by 10 from 10 to 100
for population_size in [20, 20, 20]:#range(10, 101, 10):
    print('Running test with population size: ' + str(population_size))
    args = [program_path, str(population_size), str(games), str(generations), str(mutation_rate), path_to_save]
    subprocess.run(args)


