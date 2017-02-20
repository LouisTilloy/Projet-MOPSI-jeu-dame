#include "genetic_tools.h"

#include <stdlib.h>


Grid_AI::Grid_AI(int input_size, double a10, double b10, double a20, double b20,
                 double mobility10, double mobility20, double border10,
                 double border20, double center10, double center20): Grid(input_size){
    for (int i=0; i<input_size*input_size; i++)
        tab[i] = 0;
    for (int y=0; y<input_size/2 - 1; y++){
        for (int x=0; x<input_size/2; x++){
            //cout<<x*2 + (y+1)%2 + y*input_size<<endl;
            tab[x*2 + (y+1)%2 + y*input_size] = 2;
            tab[x*2 + y%2 + (input_size-1-y)*input_size] = 1;
        }
    }
    nPieces[0] = 20;
    nPieces[1] = 20;
    nLady[0] = 0;
    nLady[1] = 0;
    piece1 = a10;
    lady1 = b10;
    piece2 = a20;
    lady2 = b20;
    mobility1 = mobility10;
    mobility2 = mobility20;
    border1 = border10;
    border2 = border20;
    center1 = center10;
    center2 = center20;
}

Grid_AI::Grid_AI(int input_size, DNA robot1, DNA robot2): Grid(input_size){
    for (int i=0; i<input_size*input_size; i++)
        tab[i] = 0;
    for (int y=0; y<input_size/2 - 1; y++){
        for (int x=0; x<input_size/2; x++){
            //cout<<x*2 + (y+1)%2 + y*input_size<<endl;
            tab[x*2 + (y+1)%2 + y*input_size] = 2;
            tab[x*2 + y%2 + (input_size-1-y)*input_size] = 1;
        }
    }
    nPieces[0] = 20;
    nPieces[1] = 20;
    nLady[0] = 0;
    nLady[1] = 0;

    // AI's DNA
    piece1 = robot1.getGene(0);
    piece2 = robot2.getGene(0);
    lady1 = robot1.getGene(1);
    lady2 = robot2.getGene(1);
    mobility1 = robot1.getGene(2);
    mobility2 = robot2.getGene(2);
    border1 = robot1.getGene(3);
    border2 = robot2.getGene(3);
    center1 = robot1.getGene(4);
    center2 = robot2.getGene(4);
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
    piece1 = game.piece1;
    lady1 = game.lady1;
    piece2 = game.piece2;
    lady2 = game.lady2;
    mobility1 = game.mobility1;
    mobility2 = game.mobility2;
    border1 = game.border1;
    border2 = game.border2;
    center1 = game.center1;
    center2 = game.center2;
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
            nLady[nPlayer-1]++; // Add 1 to the number of lady the player has
        }
    }
}

int Grid_AI::points(int player, int minmaxPlayer){
    //evaluation function of the player who started the minmax
    int piecePoints = nPieces[player-1] - nPieces[2-player];
    int ladyPoints = nLady[player-1] - nLady[2-player];
    int mobility = playables(player).size();
    double positionning = 0;
    for (int x=0; x<10; x++){
        for(int y=0; y<10; y++){
            if (tab[x+y*size] != 0 && tab[x+y*size] != 5){
                if (x == 0 || y == 0 || x == size-1 || y == size-1){
                    if (minmaxPlayer == 1)
                        positionning += (tab[x+y*size]%2 == player)*border1;
                    if (minmaxPlayer == 2)
                        positionning += (tab[x+y*size]%2 == player)*border2;
                }
                else{
                    if (minmaxPlayer == 1)
                        positionning += (tab[x+y*size]%2 == player)*center1;
                    if (minmaxPlayer == 2)
                        positionning += (tab[x+y*size]%2 == player)*center2;
                }
            }
        }
    }

    double value;
    if (minmaxPlayer == 1)
        value = piece1*piecePoints + lady1*ladyPoints + mobility1*mobility + positionning;
    if (minmaxPlayer == 2)
        value = piece2*piecePoints + lady2*ladyPoints + mobility2*mobility + positionning;
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
                new_ladies = check_ladies();
                move = minMax(3-player, minmaxPlayer, depth-1);
                mult = -1;
            }      
            for(int k=0;k<new_ladies.size();k++){
              int x_l = new_ladies[k].x;
              int y_l = new_ladies[k].y;
              set(x_l,y_l,get(x_l,y_l)-2);
            }
            go_back();
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

Move Grid_AI::alphaBeta(int player, int minmaxPlayer, int depth, int alpha, bool elag){
  Move best_move;
  if(depth==0 || isEnded()){
      best_move.setPoints(points(player, minmaxPlayer));
      return best_move;
  }
  int beta = -10000;
  bool go_on = true;
  vector<Coord> playable = playables(player);
  for(int i=0;i<playable.size();i++){
      vector<Coord> plays = availablePlays(playable[i]);
      for(int j=0;j<plays.size();j++){
          bool play_again = (availableEats(playable[i]).size()>0);
          Move move(playable[i], plays[j]);
          play(player, move);
          play_again = (play_again && availableEats(plays[j]).size()>0);
          int mult = 1;
          vector<Coord> new_ladies;
          new_ladies.clear();
          if(play_again){
              move = alphaBeta(player, minmaxPlayer, depth-1, beta, false);
          }else{
              new_ladies = check_ladies();
              move = alphaBeta(3-player, minmaxPlayer, depth -1, beta, true);
              mult = -1;
          }
          for(int k=0;k<new_ladies.size();k++){
            int x_l = new_ladies[k].x;
            int y_l = new_ladies[k].y;
            set(x_l,y_l,get(x_l,y_l)-2);
          }
          go_back();
          int points = move.getPoints();
          move.setPoints(mult*points);
          if(move.getPoints()>beta){
              beta = move.getPoints();
              best_move.init = playable[i];
              best_move.end = plays[j];
              best_move.setPoints(beta);
          }
          if(elag && (-move.getPoints() <=alpha)){
            go_on = false;
            break;
          }
      }
      if(!go_on){
        break;
      }
  }
  return best_move;
}

// ****************** DNA *******************
DNA::DNA(){
    for (int i=0; i<n_genes; i++){
        genes.push_back( (rand()%1000)/100. );
    }
    power = 0;
}

DNA::DNA(DNA parent1, DNA parent2){
    if (parent1.getSize() == parent2.getSize()){
        for (int i=0; i < parent1.getSize(); i++){
            int choice = rand()%101;
            if (choice < 50)
                genes.push_back(parent1.genes[i]);
            else if (choice < 100)
                genes.push_back(parent2.genes[i]);
            else
                genes.push_back((rand()%1000)/10.); // mutation
        }
    }
    power = 0;
}

DNA::DNA(vector<double> genes0){
    for (vector<double>::iterator it = genes0.begin(); it != genes0.end(); it++){
        genes.push_back(*it);
    }
    power = 0;
}

int DNA::getSize(){
    return genes.size();
}

double DNA::getGene(int i){
    return genes[i];
}

int DNA::getPower(){
    return power;
}
