#include "genetic_tools.h"
#include <stdlib.h>
#include <time.h>

const int depth = 3;

int main(){
    srand(time(NULL));
    rand();
    // A game between two AI
    Grid_AI game = Grid_AI(10, 1, 2);
    cout << "nPiece[0]: "<< game.nPieces[0] << endl;
    cout << "nPiece[1]: "<< game.nPieces[1] << endl;
    game.disp();
    int nPlayer = 1; // Starting player
    bool stop = false; // just a flag to stop the game in the middle
    int step = 0; //number of turns before we end the game if it is not yet ended
    while (!game.isEnded() && !stop && step<200){
        // 1 turn of the game
        Grid_AI gameTemp (game);
        // The AIs play according to the minMax algorithm
        Move move = gameTemp.minMax(nPlayer, nPlayer, depth);
        game.play(nPlayer, move);
        // The game checks if there are any piece to transform into a lady
        game.turnLady(nPlayer);
        // Next turn is played by the other player
        nPlayer = 3-nPlayer;

        // Display information
        game.disp();

        vector<Move> testmove;
        testmove.push_back(move);
        testmove[0].disp();
        cout << "move_seq.size(): "<<game.move_seq.size() <<endl;
        game.move_seq[step].disp();
        move.disp();

        //cout << "nPiece[0]: "<< game.nPieces[0] << endl;
        //cout << "nPiece[1]: "<< game.nPieces[1] << endl;
        step++;
    }
    game.disp();
    // Information to debug
    cout << "Number of turns: " << step <<endl;
    cout << game.move_seq.size() <<endl;
    cout << "can.Eat(1): "<<game.canEat(1)
         << " | can.Eat(2): "<<game.canEat(2)<<endl;

    cout << "Pieces that can move for player 1:" <<endl;
    for (int i=0; i<game.movables(1).size(); i++){
         game.movables(1)[i].disp();
    }
    cout<<endl;
    cout<< "Pieces that can move for player 2:" << endl;
    for (int i=0; i<game.movables(2).size(); i++){
         game.movables(2)[i].disp();
    }
    cout<<endl;

    return 0;
}
