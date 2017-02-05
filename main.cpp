#include "tools.h"



int main(int argc, char** argv){
    if(argc < 2){
        //Debug
        Grid G(10);
        G.copy();
        G.set(1,6,3);
        G.disp();
        Coord initCoord(1,6);
        Coord endCoord(0,5);
        Move move(initCoord, endCoord);
        G.play(1, move);
        G.disp();
        Move IA_move = G.alphaBeta(2, 4, -10000);
        G.play(2, IA_move);
        G.disp();
    }else{
        //Commande
        if(strcmp(argv[1], "grid")==0){
            //recup de la grille en argument
            Grid G(10);
            vector<int> vect;
            string str_grid = argv[2];
            int value;
            int N = str_grid.size();
            for(int i=0;i<N;i++){
                value = str_grid[i] - '0';
                if(value>=0 && value<10){
                    int a = value;
                    vect.push_back(a);
                }
            }
            G.vect_copy(vect);
            Move IA_move;
            int technique = argv[3][0] - '0';
            switch(technique){
              case 1:
              {
                int depth = argv[4][0] - '0';
                IA_move = G.minMax(2,depth);
              }
                break;
              case 2:
              {
                int nb_nodes = 30;
                IA_move = G.minMaxEq(2, nb_nodes);
              }
                break;
              case 3:
              {
                int depth = argv[4][0] - '0';
                IA_move = G.alphaBeta(2, depth, -10000);
              }
                break;
            }
            //Move IA_move = G.minMaxEq(2, 10);
            vector<Coord> IA_eats = G.eated(2, IA_move);
            send(IA_move);
            send(IA_eats);
        }if(strcmp(argv[1], "test")==0){
            cout<<"check"<<endl;
        }
    }
    return 0;
}
