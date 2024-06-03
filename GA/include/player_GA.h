#ifndef PLAYER_AI_H
#define PLAYER_AI_H

#include <array>
#include <cstring>
#include <algorithm>
#include <random>

#include "iplayer.h"

class player_ai : public iplayer
{
    private:
        std::mt19937 generator;
        std::uniform_int_distribution<int> distribution;
        float progress_pct;
        std::vector<float> genes;

    public:
        player_ai()
        {
            std::random_device rd;
            generator = std::mt19937(rd());
            progress_pct = -1;
            // genes = std::vector<float>(300,0.0);
        }

        void setChromosome(std::vector<float> _genes)
        {
            genes = _genes;
        }

        float getProgress()
        {
            return progress_pct;
        }

    private:
        int  make_decision() 
        {
            // Evaluate individual piece scores
            float pieceScore[4] = {0.0, 0.0, 0.0, 0.0};

            // Neurons
            size_t neuron_count = 24;

            for(int piece = 0; piece < 4; piece++)
            {
                // Inputs
                std::vector<bool> inputs = {
                    isQ1(piece),
                    isQ2(piece),
                    isQ3(piece),
                    isQ4(piece),
                    moveGetCaptured(piece),
                    moveCaptures(piece),
                    moveFrees(piece),
                    moveSavesSelf(piece),
                    moveSavesTeam(piece),
                    moveWins(piece),
                    enemyBehindNow(piece),
                    enemyBehindMove(piece)
                };
                size_t input_count = inputs.size();
                
                // Reset neurons
                std::vector<float> neurons = std::vector<float>(neuron_count,0.0);
                size_t gene_index = 0;
                for(size_t neuron = 0; neuron < neuron_count; neuron++)
                {
                    for(size_t input = 0; input < input_count; input++)
                    {
                        if(inputs[input])
                            neurons[neuron] += genes[gene_index];
                        gene_index++;
                    }
                }
                
                for(size_t neuron = 0; neuron < neurons.size(); neuron++)
                {
                    pieceScore[piece] += neurons[neuron]*genes[gene_index++];
                }
            }
            
            // Get best move
            float topScore = -100;
            int topIndex = -1;
            for(int i = 0; i < 4; i++)
            {
                //std::cout << 'P' << i << ": " << pieceScore[i] << ", ";
                if(pieceScore[i] > topScore)
                {
                    topScore = pieceScore[i];
                    topIndex = i;
                }
            }
            //std::cout << std::endl;

            // Return best move
            return topIndex; 
        }

        bool moveGetCaptured(int piece)
        {
            // Piece is home
            if(position[piece] == -1)
                return false;

            // Projected move
            int move = position[piece] + dice;
            move += starJump(move);

            // If piece lands on enemy on globe
            if(enemy_count(move) == 1 && isGlobe(move))
                return true;

            // If piece lands on more than one enemy
            if(enemy_count(move) > 1)
                return true;

            // Otherwise
            return false;
        }

        bool moveCaptures(int piece)
        {
            // Piece is home
            if(position[piece] == -1)
            {
                if(dice == 6 && enemy_count(0) != 0)
                    return true;
                else
                    return false;
            }

            // Projected move
            int move = position[piece] + dice;
            move += starJump(move);

            // If piece lands on one enemy not on globe
            if(!isGlobe(move) && enemy_count(move) == 1)
                return true; 

            // Otherwise
            return false;
        }

        bool moveFrees(int piece)
        {
            // Piece is home
            if(dice == 6 && position[piece] == -1)
                return true;

            // Otherwise
            return false;
        }

        bool moveSavesSelf(int piece)
        {
            // Piece is home
            if(position[piece] == -1)
                return false; //return true; // Technically... 

            // Projected move
            int move = position[piece] + dice;
            move += starJump(move);

            // If piece lands on home stretch
            if(move > 50)
                return true;

            // If piece lands on neutral globe
            if(isNeutralGlobe(move) && enemy_count(move) == 0)
                return true;

            // If piece lands on team piece
            if(team_count(move) != 0)
                return true;

            // Otherwise
            return false;
        }
        
        bool moveSavesTeam(int piece)
        {
            // Piece is home
            if(position[piece] == -1)
                return false; //return true; // Technically... 

            // Projected move
            int move = position[piece] + dice;
            move += starJump(move);

            // If piece lands on team piece
            if(team_count(move) != 0)
                return true;

            // Otherwise
            return false;
        }

        bool enemyBehindNow(int piece)
        {
            // Piece is home
            if(position[piece] == -1)
                return false;

            for(size_t roll = 1; roll <= 6; roll++)
            {
                int index = (52 + position[piece] - roll) % 52;
                for(size_t enemy = 4; enemy < 16; enemy++)
                {
                    if(position[enemy] == index)
                        return true;
                }
            }

            return false;
        }

        bool enemyBehindMove(int piece)
        {
            // Piece is home
            if(position[piece] == -1)
                return false;

            // Projected move
            int move = position[piece] + dice;
            move += starJump(move);
            
            for(size_t roll = 1; roll <= 6; roll++)
            {
                int index = (52 + move - roll) % 52;
                for(size_t enemy = 4; enemy < 16; enemy++)
                {
                    if(position[enemy] == index)
                        return true;
                }
            }

            return false;
        }

        bool moveWins(int piece)
        { 
            // Projected move
            int move = position[piece] + dice;
            move += starJump(move);

            // If move hits home
            if(move == 56)
                return true;

            return false;
        }

        bool isQ1(int piece)
        {
            return (position[piece] < 13);
        }

        bool isQ2(int piece)
        {
            return (position[piece] >= 13) && (position[piece] < 26);
        }

        bool isQ3(int piece)
        {
            return (position[piece] >= 26) && (position[piece] < 39);
        }
        bool isQ4(int piece)
        {
            return (position[piece] >= 39) && (position[piece] < 51);
        }

        float moveThreatChange(int piece)
        {
            // Projected enemy move
            int move = position[piece]+dice; 
            move += starJump(move);

            float threatCurrent = threat(position[piece]);
            float threatNew = threat(move);

            return (threatNew-threatCurrent);
        }

        // Currently only works individually 
        float threat(int index)
        {
            // If index is safe, return no threat
            if((isNeutralGlobe(index) && enemy_count(index) == 0))
            {
                return 0.0;
            }
            // Find likelyhoods of enemy killing piece 
            float p[] = {0, 0, 0, 0};
            for(int color = 1; color < 4; color++)
            {
                // For each possible dice roll
                for(int roll = 1; roll <= 6; roll++)
                {
                    for(int piece = 0; piece < 4; piece++)
                    {    
                        // Enemy piece index
                        int enemy = (color*4)+piece; 

                        // If is valid move
                        if(position[enemy] >= 0 && position[enemy] < 99)
                        {
                            // Projected enemy move
                            int move = position[enemy]+roll; 
                            move += starJump(move);

                            // If roll kills piece
                            //if(!isGlobe(move) && team_count(move) == 1)
                            if(!isGlobe(move) && move == index)
                            {
                                p[color] += (1.0/6.0);
                                break;
                            }
                        }
                    }
                }
            }

            // Union probability of 3 mutually exclusive events
            return p[1]+p[2]+p[3]-(p[1]*p[2])-(p[1]*p[3])-(p[2]*p[3])+(p[1]*p[2]*p[3]);

        }

        // Individual piece progress (percentage)
        float progress(int piece)
        {
            float pieceProgress = 0;

            if(position[piece] == 99)
                pieceProgress = 57;
            else
                pieceProgress = position[piece] + 1;

            return (pieceProgress/57.0);
        }

        bool isGlobe(int index)
        {
            if(index % 13 == 0 || index % 13 == 8)
                return true;

            return false;
        }

        bool isNeutralGlobe(int index)
        {
            if(index % 13 == 8)
                return true;

            return false;
        }

        int starJump(int index)
        {
            switch(index)
            {
                case 5:  return 6;
                case 18: return 6;
                case 31: return 6;
                case 44: return 6;

                case 11: return 7;
                case 24: return 7;
                case 37: return 7;
                case 50: return 7;

                default: return 0;
            }
        }

        int enemy_count(int index)
        {
            if(index > 50) // No enemies on home stretch (note two index 50s)
                return 0;

            int sum = 0;
            for(int i = 4; i < 16; i++)
            {
                if(position[i] == index)
                    sum++;
            }
            return sum;
        }

        int team_count(int index)
        {
            int sum = 0;
            for(int i = 0; i < 4; i++)
            {
                if(position[i] == index)
                    sum++;
            }
            return sum;
        }

        int exclusive_team_count(int index, int piece)
        {
            int sum = 0;
            for(int i = 0; i < 4; i++)
            {
                if((position[i] == index) && (i != piece))
                    sum++;
            }
            return sum;
        }

        void updateProgress() // Get simple player progress snapshot
        {
            int progress = 0;
            for(int i = 0; i < 4; i++)
            {
                if(position[i] == 99)
                {
                    progress += 57;
                }
                else
                {
                    progress += position[i]+1;
                }
            }

            // Update global progress
            progress_pct = (progress/228.0)*100;
        }

        void printPosition()
        {
            std::cout << "{ ";
            for(int i = 0; i<16; i++)
            {
                std::cout << position[i] << ", ";
            }
            std::cout << "\b\b }" << std::endl;
        }

        void moveDebug(int piece)
        {
            std::cout << "MOVE DEBUG" << std::endl;
            std::cout << "Piece: " << piece << std::endl;
            std::cout << "Dice: " << dice << std::endl;
            std::cout << "Board: ";
            printPosition();
            std::cout << std::endl;
        }
};

#endif // PLAYER_AI_H
