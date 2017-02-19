#include "genetic_tools.h"
#include <stdlib.h>
#include <time.h>

const int depth = 4;
const int poolsize = 40; // Taille de la population
const int nGenerations = 20; // Nombre de générations
const int nMatches = 4; // Nombre de matchs que fait une IA


// Creates a fight between two AI, returns the number of the winning AI
// or 0 if it's a draw.
int fight(DNA robot1, DNA robot2){
    double tab1[50], tab2[50];
    for (int i=0; i<50; i++){
        tab1[i] = robot1.getGene(3+i);
        tab2[i] = robot2.getGene(3+i);
    }
    Grid_AI game = Grid_AI(10,robot1.getGene(0), robot1.getGene(1),
                           robot2.getGene(0), robot2.getGene(1),
                           robot1.getGene(2), robot2.getGene(2),
                           tab1, tab2);
    int nPlayer = 1;
    for (int step=0; step<200; step++){
        //Move move = game.minMax(nPlayer, nPlayer, depth);
        Move move = game.alphaBeta(nPlayer, nPlayer, depth, -10000, true);
        bool play_again = game.availableEats(move.init).size()>0;
        game.play(nPlayer, move);
        play_again = play_again && game.availableEats(move.end).size()>0;
        if (!play_again){
            game.turnLady(nPlayer);
            nPlayer = 3-nPlayer;
        }
    }
    if (game.nPieces[0] == 0)
        return 2;
    else if (game.nPieces[1] == 0)
        return 1;
    else
        return 0;
}

int main(){
    srand(time(NULL));
    rand();
    /*
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
    */

    // ************ genetic algorithm *********
    vector <DNA> listDNA;
    vector<DNA> matingPool;
    for (int i=0; i<poolsize; i++){
        listDNA.push_back(DNA());
    }

    for (int i=0; i<nGenerations; i++){
        cout << "Generation :" << i << endl;
        // Making AI fight against each other
        for (int i=0; i<poolsize; i++){
            cout <<(i*100)/poolsize<<"%"<<endl;
            for(int j=0; j<nMatches; j++){
                int enemy = rand()%poolsize;
                int winner = fight(listDNA[i], listDNA[enemy]);
                if (winner == 1){
                    listDNA[i].powerup();
                    listDNA[enemy].powerdown();
                }
                else if (winner == 2){
                    listDNA[i].powerdown();
                    listDNA[enemy].powerup();
                }
            }
        }

        // Creating the mating pool
        matingPool.clear();
        for (int i=0; i<poolsize; i++){
            for(int k=0; k<listDNA[i].getPower()+1; k++){
                matingPool.push_back(listDNA[i]);
            }
        }

        // Creating the new generation of AI
        listDNA.clear();
        for (int i=0; i<poolsize; i++){
            listDNA.push_back(DNA(matingPool[rand()%poolsize],
                                  matingPool[rand()%poolsize]));
        }
    }
    cout << "Gene 1:" << listDNA[0].getGene(0) << endl
         << "Gene 2:" << listDNA[0].getGene(1) << endl
         << "Gene 3:" << listDNA[0].getGene(2) << endl;
    cout << "***********Poids des cases***************" << endl;

    for(int y=0; y<10;y++){
        cout << "-----------------------------------------------" <<endl;
        for(int x=0; x<10; x++){
            if ((x+y)%2==0)
                cout << "| X ";
            else
                cout <<"|" << listDNA[0].getGene(2 + (x+y*10+1)/2);
        }
        cout << "|" <<endl;
    }
    cout << "-----------------------------------------------"<<endl;


    return 0;
}
