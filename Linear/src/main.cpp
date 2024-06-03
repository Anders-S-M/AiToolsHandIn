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
#include "player_lin.h"
//#include "player_Ql.h"
#include "dna.h"
#include "population.h"



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


std::vector<int> parseGains(const std::vector<std::string>& args) {
    std::vector<int> gains;
    for (size_t i = 0; i < args.size(); ++i) {
        gains.push_back(std::stoi(args[i]));
    }
    return gains;
}

int main(int argc, char* argv[])
{
    // Create players
    std::vector<std::string> args(argv + 1, argv + argc);
    std::vector<int> gains = parseGains(args);

    player_lin player_0(gains);
    player_random player_1;
    player_random player_2;
    player_random player_3;

    // Create Ludo game
    game g(&player_0, &player_1, &player_2, &player_3);


    float player0Fitness = getFitness(g, 0, 2000) * 100;
    // float player1Fitness = getFitness(g, 1, 20000) * 100;
    // float player2Fitness = getFitness(g, 2, 20000) * 100;
    // float player3Fitness = getFitness(g, 3, 20000) * 100;
    //printf("Real win rate of last generation: %f\n", player0Fitness);
    // printf("Real win rate: %f\n", player1Fitness);
    // printf("Real win rate: %f\n", player2Fitness);
    // printf("Real win rate: %f\n", player3Fitness);
    // float sumsumsum = player0Fitness + player1Fitness + player2Fitness + player3Fitness;
    // printf("Sum: %f\n", sumsumsum);

    // End of main
    std::cout << player0Fitness << std::endl;
    return 0;
}
