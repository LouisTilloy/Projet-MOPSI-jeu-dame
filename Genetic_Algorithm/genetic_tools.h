#ifndef GENETIC_H
#define GENETIC_H

#include <iostream>
#include <vector>
#include "../tools.h"

using namespace std;

// 0: no piece on the square
// 1: player 1 piece
// 2: player 2 piece
// 3: player 1 lady
// 4: player 2 lady
// 5: square is outsitde the grid

const int n_genes = 3;

class Grid_AI: public Grid{
public:
    int a1, a2; // Coeficients giving the importance of the criteria
              // nLady[nPlayer-1]-nLady[2-nPlayer] and
              // the criteria nPiece[nPlayer-1]-nPiece[2-nPlayer]
              // -------- /!\ only for the player 2 for now -------
    Grid_AI(int input_size, int a10, int a20);
    Grid_AI(const Grid_AI &game);
    ~Grid_AI();
    void turnLady(int nPlayer);
    // Overloading function points
    int points(int player, int minmaxPlayer);
    // Overloading minmax function
    Move minMax(int player, int minmaxPlayer, int depth);
};

class DNA{
    vector<int> genes;

};

#endif // GENETIC_H
