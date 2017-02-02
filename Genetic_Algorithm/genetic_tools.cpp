#include "genetic_tools.h"

#include <stdlib.h>


Grid_AI::Grid_AI(int input_size, int a10, int a20): Grid(input_size){
    for (int i=0; i<input_size*input_size; i++)
        tab[i] = 0;
    for (int y=0; y<input_size/2 - 1; y++){
        for (int x=0; x<input_size/2; x++){
            cout<<x*2 + (y+1)%2 + y*input_size<<endl;
            tab[x*2 + (y+1)%2 + y*input_size] = 2;
            tab[x*2 + y%2 + (input_size-1-y)*input_size] = 1;
        }
    }
    nPieces[0] = 20;
    nPieces[1] = 20;
    nLady[0] = 0;
    nLady[1] = 0;
    a1 = a10;
    a2 = a20;
}

Grid_AI::Grid_AI(const Grid_AI &game){
    size = game.size;
    tab = new int[size*size];
    for (int i=0; i<size*size; i++){
        tab[i] = game.tab[i];
    }
    ended = game.ended;
    nPieces[0] = game.nPieces[0];
    nPieces[1] = game.nPieces[1];
    nLady[0] = game.nLady[0];
    nLady[1] = game.nLady[1];
    move_seq = game.move_seq;
    eat_seq = game.eat_seq;
    a1 = game.a1;
    a2 = game.a2;
}

Grid_AI::~Grid_AI(){
    delete [] tab;
}

void Grid_AI::turnLady(int nPlayer){
    // Transform all the pieces of nPlayer that went to the other side
    // of the bord into ladies
    int y;
    if (nPlayer == 1)
        y = 0;
    else
        y = size - 1;
    for(int x=0; x<size;x++){
        if (get(x,y) == nPlayer){
            set(x, y, nPlayer+2); // turns the piece into a lady
            nLady[nPlayer]++; // Add 1 to the number of lady the player has
        }
    }
}

int Grid_AI::points(int player, int minmaxPlayer){
    return rand()%100;
    //evaluation function of the player who started the minmax
    int piecePoints = nPieces[player-1] - nPieces[2-player];
    int ladyPoints;
    if (minmaxPlayer == 2)
        ladyPoints = nLady[player-1] - nLady[2-player];
    else
        ladyPoints = 0;
    int value = a1*piecePoints + a2*ladyPoints;
    return value;
}

Move Grid_AI::minMax(int player, int minmaxPlayer, int depth){
    Move best_move;
    if(depth==0 || isEnded()){
        best_move.setPoints(points(player, minmaxPlayer));
        return best_move;
    }
    int max_move = -10000;
    vector<Coord> playable = playables(player);
    for(int i=0;i<playable.size();i++){
        vector<Coord> plays = availablePlays(playable[i]);
        for(int j=0;j<plays.size();j++){
            bool play_again = (availableEats(playable[i]).size()>0);
            Move move(playable[i], plays[j]);
            play(player, move);
            play_again = (play_again && availableEats(plays[j]).size()>0);
            vector<Coord> new_ladies;
            int mult = 1;
            if(play_again){
                move = minMax(player, minmaxPlayer, depth-1);
            }else{
                move = minMax(3-player, minmaxPlayer, depth-1);
                mult = -1;
            }
            go_back();
            for(int k=0;k<new_ladies.size();k++){
              int x_l = new_ladies[k].x;
              int y_l = new_ladies[k].y;
              set(x_l,y_l,get(x_l,y_l)-2);
            }
            int points = move.getPoints();
            move.setPoints(mult*points);
            if(move.getPoints()>max_move){
                max_move = move.getPoints();
                best_move.init = playable[i];
                best_move.end = plays[j];
                best_move.setPoints(max_move);
            }
        }
    }
    return best_move;
}
