#ifndef PLAYER_ql_H
#define PLAYER_ql_H

#include <array>
#include <cstring>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <limits>

#include "iplayer.h"

class player_ql : public iplayer
{
    private:
        std::mt19937 generator;
        std::uniform_real_distribution<double> distribution;
        float progress_pct;
        std::unordered_map<int, std::vector<double>> q_table;
        double learning_rate;
        double discount_factor;
        double epsilon;
        int lastreward = 0;
        int laststate = 0;
        int lastaction = 0;

    public:
        player_ql()
            : generator(std::random_device{}()), distribution(0.0, 1.0), progress_pct(-1),
            learning_rate(0.1), discount_factor(0.9), epsilon(0.1)
        {

        }


        float getProgress()
        {
            return progress_pct;
        }

    private:
        int bool_vector_to_int(const std::vector<bool>& inputs)
        {
            int state = 0;
            for (size_t i = 0; i < inputs.size(); ++i) {
                state |= (inputs[i] << i);
            }
            return state;
        }

        void initialize_q_values(int state)
        {
            if (q_table.find(state) == q_table.end()) {
                q_table[state] = std::vector<double>(4, 0.0); // 4 actions (one for each piece)
            }
        }

        int get_action(int state)
        {
            initialize_q_values(state);
            if (distribution(generator) < epsilon) {
                // Explore: random action
                std::uniform_int_distribution<int> action_distribution(0, 3);
                return action_distribution(generator);
            } else {
                // Exploit: best action based on Q-values
                return std::distance(q_table[state].begin(), std::max_element(q_table[state].begin(), q_table[state].end()));
            }
        }

        int get_state() // Make a number based on the position of each piece
        {
            return (position[0] << 24) | (position[1] << 16) | (position[2] << 8) | position[3];
        }

        void update_q_values(int state, int action, double reward, int new_state)
        {
            initialize_q_values(state);
            initialize_q_values(new_state);

            double max_future_q = *std::max_element(q_table[new_state].begin(), q_table[new_state].end());
            q_table[state][action] += learning_rate * (reward + discount_factor * max_future_q - q_table[state][action]);
        }

        int get_reward(int piece){
            int sum = 0;
            sum =- moveDies(piece) * -5;
            sum =+ moveKills(piece) * 4;
            sum =+ moveFrees(piece);
            sum =+ moveSavesSelf(piece);
            sum =+ moveSavesTeam(piece);
            sum =+ moveWins(piece) * 5;
            sum =+ enemyBehindMove(piece);
            return sum;
        }


        int make_decision()
        {
            int state = get_state();

            // Update Q-values
            update_q_values(laststate, lastaction, lastreward, state);

            // Select action based on Q-values
            int action = get_action(state);

            // Make a state from positions of pieces
            lastreward = get_reward(action);

            lastaction = action;
            
            // Return the best action (the piece to move)
            return action;
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

#endif // PLAYER_ql_H
