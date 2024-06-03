import subprocess



print('Compiling C program...')
subprocess.run(['make'])


program_path = 'build/ludo'
print('Running test...')


#test games, constant population size and generation and mutation rate
population_size = 40
mutation_rate = 1.0
generations = 2000
path_to_save = 'logs/Games/'

#increment games by 100 from 1000 to 100
for games in [100]:#range(10, 51, 10):
    print('Running test with population games: ' + str(games))
    args = [program_path, str(population_size), str(games), str(generations), str(mutation_rate), path_to_save]
    subprocess.run(args)