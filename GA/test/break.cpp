#include <iostream>

int main()
{
    for (int roll = 1; roll <= 6; roll++)
    {
        for(int piece = 0; piece < 4; piece++)
        {
            if(piece == 2 && roll == 5)
                break;
            std::cout << "Roll: " << roll << ", Piece: " << piece << std::endl;
        }
    }
    return 0;
}
