#include "tools.h"



int main(int argc, char** argv){
    if(argc < 2){
        //Debug
        Grid G(10);
        G.copy();
        G.disp();
        Coord initCoord(1,6);
        Coord endCoord(0,5);
        Move move(initCoord, endCoord);
        G.play(1, move);
        G.disp();
        Move IA_move = min_max(G, 2, 1);
        G.play(2, IA_move);
        G.disp();
    }else{
        //Commande
        if(strcmp(argv[1], "grid")){
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
        }
    }
    return 0;
}



