#ifndef PLAYER_BOOL_H
#define PLAYER_BOOL_H

#include "iplayer.h"
#include "move_logic.h"


class player_bool : public iplayer
{
private:
    move_logic logic;

public:
    player_bool() { /* do nothing */ }

private:
    int make_decision()
    {
        int piece = -1;
        logic.set(dice, position);

        if(logic.count_valid_moves() < 2)    // If no choice
            return logic.first_legal_move(); // Don't waste time analysing


        //Else do almost like 'player_fast'
        piece = logic.move_into_goal();     if(piece != -1) return piece; // Bring a piece into goal
        piece = logic.move_send_home();     if(piece != -1) return piece; // Send an opponent home, go for maximum damage if multiple options
        piece = logic.move_start();         if(piece != -1) return piece; // Bring a piece into play
        piece = logic.move_fast_safe();     if(piece != -1) return piece; // Move the most advanced outfield piece posible without sending itself home
        piece = logic.move_random_safe();   if(piece != -1) return piece; // Make any move which don't send ourself home.
        piece = logic.move_early_outfield();if(piece != -1) return piece;// Forced to send ourself home, we choose the least advanced one.
        piece = logic.first_legal_move();   if(piece != -1) return piece;

        return -1; //Should never reach this line. Provided for a sense of completeness
    }
};

#endif