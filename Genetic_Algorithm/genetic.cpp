#include "genetic_tools.h"
#include <stdlib.h>
#include <time.h>

const int depth = 3;

int main(){
    srand(time(NULL));
    rand();
    // A game between two AI
    Grid_AI game = Grid_AI(10, 1, 100);
    cout << "nPiece[0]: "<< game.nPieces[0] << endl;
    cout << "nPiece[1]: "<< game.nPieces[1] << endl;
    game.disp();
    int nPlayer = 1; // Starting player
    bool stop = false; // just a flag to stop the game in the middle
    int step = 0; //number of turns before we end the game if it is not yet ended
    while (!game.isEnded() && !stop && step<200){
        // 1 turn of the game
        // The AIs play according to the minMax algorithm
        Move move = game.minMax(nPlayer, nPlayer, depth);
        bool play_again = game.availableEats(move.init).size()>0;
        //cout<<endl;
        //game.move_seq[step].disp();
        game.play(nPlayer, move);
        play_again = play_again && game.availableEats(move.end).size()>0;
        // The game checks if there are any piece to transform into a lady
        // Next turn is played by the other player
        if(!play_again){
          game.turnLady(nPlayer);
          nPlayer = 3-nPlayer;
        }

        // Display information
        game.disp();
        cout << "nPiece[0]: "<< game.nPieces[0] << endl;
        cout << "nPiece[1]: "<< game.nPieces[1] << endl;
        cout << "nLady[0]: "<< game.nLady[0] << endl;
        cout << "nLady[1]: "<< game.nLady[1] << endl;
        cout << game.move_seq.size() <<endl;
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
