#ifndef TOOLS_H
#define TOOLS_H


#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <string.h>
#include <time.h>
using namespace std;


string iToStr(int a);
//converts int into string
//(I haven't really found easier ways)
int strToInt(string s);
//converts string into int (same thing)

class Coord{
    //field that can tell if there is a piece on this Coord, to which
    //player it belongs and if it's a Lady
    //it is only used in one or two functions, to help remember what were
    //the pieces that were eaten (player, Lady) when you only have a vector of Coord of the eats
    int type;
public:
  int x, y;
  Coord(){
      type = 0;
  }
  Coord(int x0, int y0){
        type = 0;
        x = x0;
        y = y0;
  }
  void disp(){
    cout<<"("<<x<<", "<<y<<")"<<endl;
  }
  void operator=(Coord C){
    x = C.x;
    y = C.y;
  }
  int getType(){
      return type;
  }
  int setType(int in_type){
      type = in_type;
  }
};


//same as in python
//this one is used for the moves
vector<Coord> diag_large(Coord init, Coord end);
//this one is used for the eats (only considers the Coords strictly between init and end)
vector<Coord> diag(Coord init, Coord end);


class Move{
  //this field is one way I could think of to carry the evaluation of a move all along the minMax algorithm
  //the more "points" is high, the more the player should play this move
  int points;
public:
  Coord init, end;
  Move(Coord input_init, Coord input_end){
    setCoords(input_init, input_end);
  }
  Move(){
      Coord C_init(0,0);
      Coord C_end(0,0);
      init = C_init;
      end = C_end;
      points = 0;
  }
  void setPoints(int input_points){
    points = input_points;
  }
  void setCoords(Coord input_init, Coord input_end){
    init = input_init;
    end = input_end;
  }
  int getPoints(){
    return points;
  }
  Coord getInit(){
    return init;
  }
  Coord getEnd(){
    return end;
  }
  void operator=(Move M){
      init = M.init;
      end = M.end;
      points = M.points;
  }

  void disp(){
    cout<<"[("<<init.x<<", "<<init.y<<"), ("<<end.x<<", "<<end.y<<")]"<<endl;
  }
};


//not usefull at all
int occupy_type(Coord C);
//get the information of the game from the python's script
vector<int> getGrid();


class Grid{
protected:
  //Where we keep all the data from the game
  int* tab;
  int size;
  bool ended;
public:
  int nPieces[2];
  int nLady[2];
  //vector of all the lasts moves (so that we can go back as many times as we want)
  vector<Move> move_seq;
  //vector of all the last eats (to recreate the pieces eaten when we go back)
  vector<vector<Coord> > eat_seq;
//public:
  Grid(){}
  ~Grid(){ delete[] tab;}

  Grid(int inputSize){
    size = inputSize;
    tab = new int[size*size];
    nPieces[0] = 0;
    nPieces[1] = 0;
    nLady[0] = 0;
    nLady[1] = 0;
    move_seq.clear();
    eat_seq.clear();
  }
  int getPlayer(int x, int y){
    //gives the number of the player to whome belongs the piece at (i, j)
    if(get(x, y)==1 || get(x, y)==3){
        return 1;
    }else if(get(x, y)==2 || get(x, y)==4){
        return 2;
    }
    return 0;
  }
  int getPlayer(Coord C){
    //same but with Coord
    return getPlayer(C.x, C.y);
  }
  void vect_copy(vector<int> V){
      for(int i=0;i<V.size();i++){
          tab[i] = V[i];
          if(V[i]>0){
              if(V[i]%2==0){
                  nPieces[1]++;
              }else{
                  nPieces[0]++;
              }
          }
      }
  }

  int operator()(Coord C){
    return get(C.x, C.y);
  }
  int operator[](Coord C){
      return get(C.x, C.y);
  }

  int operator()(int x, int y){
    return get(x, y);
  }
  void set(int x, int y, int s){
    if(x>=0 && y>=0 && x<size && y<size){
        tab[x*size+y] = s;
    }
  }
  int get(int x, int y){
    //classic get
    if(y<0 || x<0 || y>=size || x>=size){
        return 5;
    }
    return tab[x*size+y];
  }
  int get(Coord C){
    return get(C.x, C.y);
  }
  void copy(){
    //obsolete but we never know
    //copy the game from the python script
    vector<int> tab_grid = getGrid();
    vect_copy(tab_grid);
  }
  void disp(){
    //displays the grid (for debug)
    cout<<"["<<endl;
    for(int y=0;y<size;y++){
      cout<<"[";
      for(int x=0;x<size;x++){
        if(x>0){
          cout<<", ";
        }
        cout<<get(x, y);
      }
      cout<<"]"<<endl;
    }
    cout<<"]"<<endl;
  }
  string putStr(){
      //puts all the necessary informations of the grid in
      //a string so that we can communicate those informations to
      //th python script by a cout
      string res = "";
      for(int y=0;y<size;y++){
          for(int x=0;x<size;x++){
              res += iToStr(get(x, y));
          }
      }
      return res;
  }
  bool isEnded(){
      //tells if the game is finished
      return (nPieces[0]<=0 || nPieces[1]<=0);
  }
  int points(int player){
      //evaluation function that returns the diff between the player's number of pieces and
      //and his adversary's
      return nPieces[player-1] - nPieces[2-player];
  }

  vector<Coord> eated(int player, Move move){
      //returns the coords of the pieces that are eaten when the player does a certain move
      vector<Coord> diagonale = diag(move.init, move.end);
      vector<Coord> vect;
      vect.clear();
      for(int i=0;i<diagonale.size();i++){
          Coord C = diagonale[i];
          if((*this)(C) == 3-player){
              C.setType(3-player);
              vect.push_back(C);
          }else if((*this)(C) == 3-player+2){
              C.setType(3-player+2);
              vect.push_back(C);
          }
      }
      return vect;
  }
  void play(int player, Move move){
      //changes the grid according to the move given in parameter
      vector<Coord> eats = eated(player, move);
      nPieces[2-player] -= eats.size();
      for(int i=0;i<eats.size();i++){
          if ( get(eats[i].x, eats[i].y) == 2-player + 2 )
              nLady[2-player] --;
          set(eats[i].x, eats[i].y, 0);
      }
      int a = get(move.init.x, move.init.y);
      set(move.init.x, move.init.y, 0);
      set(move.end.x, move.end.y, a);
      eat_seq.push_back(eats);
      move_seq.push_back(move);
  }
  void go_back(){
      //Cancel the last move
      vector<Coord> eats = eat_seq.back();
      eat_seq.pop_back();
      Move move = move_seq.back();
      move_seq.pop_back();
      int eated_player;
      for(int i=0;i<eats.size();i++){
          set(eats[i].x, eats[i].y, eats[i].getType());
          eated_player = eats[i].getType();

          nPieces[1-(eated_player%2)]++;
      }
      int a = get(move.end.x, move.end.y);
      set(move.end.x, move.end.y, 0);
      set(move.init.x, move.init.y, a);
  }

  vector<Coord> check_ladies(){
    vector<Coord> new_ladies;
      for(int x=0;x<size;x++){
          if(get(x,0)==1){
              set(x,0,3);
              Coord C(x,0);
              new_ladies.push_back(C);
          }
          if(get(x,size-1)==2){
              set(x,size-1,4);
              Coord C(x,size-1);
              new_ladies.push_back(C);
          }
      }
      return new_ladies;
  }
  //same as in python
  vector<Coord> availableMoves(Coord start);
  vector<Coord> movables(int player);
  vector<Coord> availableEats(Coord start);
  vector<Coord> availableEaters(int player);
  bool canEat(int player);
  vector<Coord> playables(int player);
  vector<Coord> availablePlays(Coord start);
  Move minMax(int player, int depth);
  Move minMaxEq(int player, int nodes);
  Move alphaBeta(int player, int depth, int alpha, bool elag);
};

//function that communicate with the python script, telling it what to do
string action(string actionName);


//function that runs a system command and returns the output
string exec(const char* cmd);

//All those function are based on the communication with the python script and therefore are
//much slower than the Grid's methodes that do the same thing but only in c++
vector<Move> getMoves(Coord start, Grid G);
vector<Coord> getMovables(int player, Grid G);
vector<Coord> getEaters(int player, Grid G);
vector<Coord> getEats(Coord start, Grid G);
Move min_max(Grid& G, int player, int depth);
bool canEat(int player, Grid G);
vector<Coord> getPlayables(int player, Grid G);
int play(int player, Move move, Grid G);
vector<Coord> possiblePlays(int player, Coord start, Grid G);

//function that will send the output of the IA to the python's script:
//sending the move to do
void send(Move move);
//sending the eventual eats implied by the move
void send(vector<Coord> eats);

#endif //TOOLS_H
