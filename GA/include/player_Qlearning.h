#ifndef PLAYER_Qlearning_H
#define PLAYER_Qlearning_H

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
        std::unordered_map<std::string, std::vector<float>> q_table;
        float learning_rate = 0.1;
        float discount_factor = 0.99;
        float epsilon = 0.1;  // Epsilon-greedy parameter
        std::vector<int> position;
        int dice;

    public:
        player_ai()
        {
            std::random_device rd;
            generator = std::mt19937(rd());
            progress_pct = -1;
            q_table.clear();
            position = std::vector<int>(16, -1);
        }

        float getProgress()
        {
            return progress_pct;
        }

        void setDice(int d)
        {
            dice = d;
        }

        void setPosition(const std::vector<int>& pos)
        {
            position = pos;
        }

    private:
        std::string get_state(int piece)
        {
            std::string state = std::to_string(position[piece]) + "," + std::to_string(dice);
            return state;
        }

        int make_decision()
        {
            int best_action = 0;
            float best_value = -std::numeric_limits<float>::infinity();

            std::vector<int> actions = {0, 1, 2, 3};
            std::shuffle(actions.begin(), actions.end(), generator);

            for (int piece : actions)
            {
                std::string state = get_state(piece);
                if (q_table[state].empty())
                {
                    q_table[state] = std::vector<float>(4, 0.0);  // Initialize Q-values for all actions
                }

                float q_value = q_table[state][piece];
                if (q_value > best_value)
                {
                    best_value = q_value;
                    best_action = piece;
                }
            }

            if (distribution(generator) < epsilon)
            {
                best_action = actions[distribution(generator) % actions.size()];  // Explore: Random action
            }

            return best_action;
        }

        void update_q_value(int piece, float reward, const std::vector<int>& new_position, int new_dice)
        {
            std::string state = get_state(piece);
            std::string new_state = get_state(piece);  // New state after taking action
            float max_future_q = *std::max_element(q_table[new_state].begin(), q_table[new_state].end());
            float current_q = q_table[state][piece];
            float new_q = (1 - learning_rate) * current_q + learning_rate * (reward + discount_factor * max_future_q);
            q_table[state][piece] = new_q;
        }

        // Reward function can be implemented based on the game logic
        float get_reward(int piece)
        {
            // Simplified reward function
            if (moveWins(piece))
                return 5.0;
            if (moveKills(piece))
                return 0.5;
            if (moveDies(piece))
                return -0.5;
            if (moveFrees(piece))
                return 0.5;
            return 0.0;
        }

        bool moveDies(int piece)
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

        bool moveKills(int piece)
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

        bool enemyBehind(int piece)
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
