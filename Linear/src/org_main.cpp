#include <iostream>
#include <random>
#include <fstream>
#include <string>
#include <iomanip>

#include "game.h"
#include "player_random.h"
#include "player_random_safe.h"
#include "player_aggro_fast.h"
#include "player_fast.h"
#include "player_ai.h"
#include "player_multi_agent.h"
#include "dna.h"
#include "population.h"

float getFitness(game g, int color, size_t iterations)
{
    // Play game(s) and get win count
    int wins = 0;
    for (size_t i = 0; i < iterations; i++)
    {
        g.reset();
        g.set_first(i % 4);
        g.play_game();
        if (g.get_winner() == color)
            wins++;
    }

    // Return win percentage
    return wins / (iterations * 1.0);
}

int main(int argc, char **argv)
{
    bool logging = false;
    if (argc > 1)
    {
        std::string argument = argv[1];

        if (argument == "-l")
            logging = true;
    }

    // Create multi agent input
    player_random multi_agent0;
    player_fast multi_agent1;
    player_aggro_fast multi_agent2;
    player_random_safe multi_agent3;
    std::vector<iplayer *> multi_agents = {&multi_agent0, &multi_agent1, &multi_agent2, &multi_agent3};

    // Create players
    player_ai player_0;
    player_fast player_1;
    player_fast player_2;
    player_fast player_3;

    // Create Ludo game
    game g(&player_0, &player_1, &player_2, &player_3);

    // Play some games (testing)
    /*
    int wins[] = { 0, 0, 0, 0 };
    for(int i = 0; i < 1000; i++)
    {
        g.reset();
        g.set_first(i%4);
        g.play_game();
        wins[g.get_winner()]++;
    }
    for(int i = 0; i < 4; i++)
        std::cout << "Player " << i << " won " << wins[i] << " games." << std::endl;
    */
    /*
    std::ofstream myfile;
    myfile.open("output.csv");
    myfile << "iterations,winrate\n";
    std::cout << std::endl;
    for(int i = 0; i <= 1000; i+=2)
    {
        for(int j = 0; j < 10; j++)
        {
            float fitness = getFitness(g,0,i)*100;
            myfile << i << ',' << fitness << '\n';
            std::cout << i << "    \r";
        }
    }
    myfile.close();
    */

    // Training parameters
    DNA root(std::vector<float>(300, 0.0));
    size_t population_size = 40;
    size_t game_count = 600;
    float mutation_rate = 0.9;
    size_t generations = 1500;

    std::ofstream myfile;
    for (int i = 0; i < 10;)
    {
        if (logging)
        {
            int j = 2;
            std::cout << "Logging..." << std::endl;
            std::string name =
                "size" + std::to_string(population_size) +
                "_iterations" + std::to_string(game_count) +
                "_mutation" + std::to_string(mutation_rate) +
                "_generation" + std::to_string(generations) +
                "_number" + std::to_string(j);

            myfile.open("/home/student/AI/AI_tools_ludo/logs/Pop/" + name + ".csv", std::ofstream::out | std::ofstream::app);
            myfile << "\n";
        }

        // Create initial random population from root gene
        Population population;

        // Run genetic algorithm
        for (size_t i = 0; i < generations + 1; i++)
        {
            std::cout << "Generation #" << i;

            // Update population
            if (i == 0)
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

            // Calculate missing gene fitness
            for (size_t i = 0; i < population.getSize(); i++)
            {
                if (population.members[i].fitness == -1)
                {
                    player_0.setChromosome(population.members[i].genes);
                    population.members[i].fitness = getFitness(g, 0, game_count);
                }
            }

            // Eliminate two worst individuals
            // Note: Not entirely by textbook. Should be updated.
            population.elimination(2);

            // print debug outpution
            float avgFit = population.avgFitness() * 100;
            if (logging)
            {

                myfile << avgFit << ",";
            }
            std::cout << "\t(" << avgFit << "%)" << std::endl;
            // population.print();
        }

        if (logging)
            i++;
        myfile.close();
    }

    // End of main
    std::cout << "End of main" << std::endl;
    return 0;
}
