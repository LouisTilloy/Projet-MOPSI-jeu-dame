#include "tools.h"



int main(int argc, char** argv){
    if(argc < 2){
        //Debug
        Grid G(10);
        G.copy();
        for(int i=0;i<10;i++){
          for(int j=0;j<10;j++){
            G.set(i,j,0);
          }
        }
        G.set(1,0,4);
        G.set(3,2,1);
        G.set(4,3,1);
        G.disp();
        string ch = "y";
        while(ch=="y"){
          Move IA_move = G.bestLimitedAnswer(2, 2000);
          IA_move.disp();
          G.play(1, IA_move);
          G.disp();
          IA_move = G.bestLimitedAnswer(1, 2000);
          G.play(2, IA_move);
          G.disp();
          cout<<G.isEnded()<<endl;
          cout<<"continue?  "<<endl;
          cin>>ch;
        }

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
                IA_move = G.alphaBeta(2, depth, -10000, false);
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
