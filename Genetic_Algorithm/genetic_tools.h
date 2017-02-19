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

const int n_genes = 53;

class Grid_AI: public Grid{
public:
    double piece1, lady1, piece2, lady2; // Coeficients giving the importance of the criteria
              // nLady[nPlayer-1]-nLady[2-nPlayer] and
              // the criteria nPiece[nPlayer-1]-nPiece[2-nPlayer]
    double mobility1, mobility2; // coeficient giving the importance of the number of possible play
    double positions1[50], positions2[50]; // coeficients giving the importance of the position
    Grid_AI(int input_size, double a10, double b10, double a20, double b20,
            double mobility10, double mobility20, double positions10[50],
            double positions20[50]);
    Grid_AI(const Grid_AI &game);
    ~Grid_AI();
    void turnLady(int nPlayer);
    // Overloading function points
    int points(int player, int minmaxPlayer);
    // Overloading minmax function
    Move minMax(int player, int minmaxPlayer, int depth);
    // Overloading alpha beta function
    Move alphaBeta(int player, int minmaxPlayer, int depth, int alpha, bool elag);
};

class DNA{
    vector<double> genes;
    int power;
public:
    DNA(); // Creates random genes
    DNA(DNA parent1, DNA parent2);
    DNA(vector<double> genes0);
    void powerup(){ power ++; }
    void powerdown(){ power --; }
    int getSize();
    double getGene(int i);
    int getPower();
};



#endif // GENETIC_H
