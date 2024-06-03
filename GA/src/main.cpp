#include <iostream>
#include <random>
#include <fstream>
#include <string>
#include <iomanip>
#include <thread>
#include <unistd.h>
#include <chrono>

#include "game.h"
#include "player_random.h"
#include "player_random_safe.h"
#include "player_aggro_fast.h"
#include "player_fast.h"
#include "player_GA.h"
#include "dna.h"
#include "population.h"


long numthreads = sysconf(_SC_NPROCESSORS_ONLN);

float getFitness(game g, int color, size_t games)
{
    // Play game(s) and get win count
    int wins = 0;
    for (size_t i = 0; i < games; i++)
    {
        g.reset();
        g.set_first(i % 4);
        g.play_game();
        if (g.get_winner() == color)
            wins++;
    }

    // Return win percentage
    return wins / (games * 1.0);
}


void startRange(size_t start, size_t end, Population &population, player_ai &player_0, game &g, int game_count)
{
    // printf("\nThread from: %ld to: %ld", start, end-1);
    for (size_t i = start; i < end; i++)
    {
        // printf("\nThread: %ld", i);
        if (population.members[i].fitness == -1)
        {
            player_0.setChromosome(population.members[i].genes);
            population.members[i].fitness = getFitness(g, 0, game_count);
        }
    }
}

int main(int argc, char **argv)
{
    printf("Number of threads: %ld\n", numthreads);
    bool logging = true;
    bool multiProcessing = true;
 
    if (argc != 6)
    {
        std::cout << "Usage: ./main population_size game_count generations mutation_rate path_to_save" << std::endl;
        return 1;
    }

    int population_size = std::stoi(argv[1]);
    int game_count = std::stoi(argv[2]);
    int generations = std::stoi(argv[3]);
    float mutation_rate = std::stof(argv[4]);
    std::string path_to_save = argv[5];


    // Create players
    player_ai player_0;
    player_random player_1;
    player_random player_2;
    player_random player_3;

    // Create Ludo game
    game g(&player_0, &player_1, &player_2, &player_3);

    // Training parameters
    DNA root(std::vector<float>(312, 0.0));

    std::ofstream myfile;
    int runs = 10;
    for (int i = 0; i < runs;)
    {
        printf("Training number: %d\n", i + 1);
        if (logging)
        {
            std::cout << "Logging..." << std::endl;
            std::string name =
                "PopulationSize" + std::to_string(population_size) +
                "_gameCount" + std::to_string(game_count) +
                "_mutationRate" + std::to_string(mutation_rate) +
                "_generations" + std::to_string(generations) +
                "_run" + std::to_string(i+1);
            std::string path = path_to_save + name + ".csv";

            myfile.open(path, std::ofstream::out | std::ofstream::app);
            printf("Logging to %s\n", path.c_str());
        }

        // Create initial random population from root gene
        Population population;

        // Run genetic algorithm
        for (size_t gen = 0; gen < generations + 1; gen++)
        {

            // std::cout << "Generation #" << gen;

            // Update population

            if (gen == 0)
            {
                // Random population if none exists
                population.random(population_size, root);
            }
            else
            {
                // Select two best (roulette) individuals
                population.selection();

                // Generate two children
                population.crossoverUniform(mutation_rate);
            }

            ////////////////////////////
            // Calculate missing gene fitness
            // Split the iteration range into smaller ranges

            if (multiProcessing)
            {

                size_t chunkSize = population.getSize() / numthreads;
                size_t remainingElements = population.getSize() % numthreads;


                // Create and start the threads
                std::vector<std::thread> threads;
                size_t start = 0;
                size_t end = chunkSize;

                for (int t = 0; t < numthreads; t++)
                {
                    if (t <= remainingElements)
                        end++;
                    threads.emplace_back(startRange, start, end, std::ref(population), std::ref(player_0), std::ref(g), game_count);
                    start = end;
                    end += chunkSize;
                }

                // Wait for all threads to finish
                for (auto &thread : threads)
                {
                    thread.join();
                }
            }
            else
            {
                for (size_t t = 0; t < population.getSize(); t++)
                {
                    // Evaluate each member
                    if (population.members[t].fitness == -1) 
                    {
                        player_0.setChromosome(population.members[t].genes);
                        population.members[t].fitness = getFitness(g, 0, game_count);
                    }
                }
            }

            // find max fitness
            float maxFitness = 0;
            for (size_t t = 0; t < population.getSize(); t++)
            {
                if (population.members[t].fitness > maxFitness)
                {
                    maxFitness = population.members[t].fitness;
                }
            }


            // print debug outpution
            float avgFit = population.avgFitness() * 100;
            if (logging)
            {
                //printf("\t(%f)%%\n", avgFit); // Print avg fit
                myfile << avgFit << ", ";
            }

            population.elimination(2); // Not needed as only the 2 best are used anyways and it messes with plots
            // population.print();
        }
        if (logging)
            i++;
        myfile << "\n";
        myfile.close();

        float avgFit = population.avgFitness() * 100;
        printf("Avg:(%f)%%\n", avgFit);

        population.selection();
        player_0.setChromosome(population.get_fittest().genes);
        float player0Fitness = getFitness(g, 0, 20000) * 100;
        // float player1Fitness = getFitness(g, 1, 20000) * 100;
        // float player2Fitness = getFitness(g, 2, 20000) * 100;
        // float player3Fitness = getFitness(g, 3, 20000) * 100;
        printf("Real win rate of last generation: %f\n", player0Fitness);
        // printf("Real win rate: %f\n", player1Fitness);
        // printf("Real win rate: %f\n", player2Fitness);
        // printf("Real win rate: %f\n", player3Fitness);
        // float sumsumsum = player0Fitness + player1Fitness + player2Fitness + player3Fitness;
        // printf("Sum: %f\n", sumsumsum);
    }

    // End of main
    std::cout << "End of main" << std::endl;
    return 0;
}
