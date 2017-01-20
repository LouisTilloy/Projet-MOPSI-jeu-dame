#ifndef TOOLS_H
#define TOOLS_H

#endif //TOOLS_H

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
  Coord(int i, int j){
        type = 0;
        x = i;
        y = j;
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
  //Where we keep all the data from the game
  int* tab;
  int size;
  bool ended;
  int nPieces[2];
  //vector of all the lasts moves (so that we can go back as many times as we want)
  vector<Move> move_seq;
  //vector of all the last eats (to recreate the pieces eaten when we go back)
  vector<vector<Coord> > eat_seq;
public:
  Grid(int inputSize){
    size = inputSize;
    tab = new int[size*size];
    nPieces[0] = 0;
    nPieces[1] = 0;
    move_seq.clear();
    eat_seq.clear();
  }
  int getPlayer(int i, int j){
    //gives the number of the player to whome belongs the piece at (i, j)
    if(get(i, j)==1 || get(i, j)==3){
        return 1;
    }else if(get(i, j)==2 || get(i, j)==4){
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

  int operator()(int i, int j){
    return get(i, j);
  }
  void set(int i, int j, int s){
    if(i>=0 && j>=0 && i<size && j<size){
        tab[i*size+j] = s;
    }
  }
  int get(int i, int j){
    //classic get
    if(i<0 || j<0 || i>=size || j>=size){
        return 5;
    }
    return tab[i*size+j];
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
    for(int i=0;i<size;i++){
      cout<<"[";
      for(int j=0;j<size;j++){
        if(j>0){
          cout<<", ";
        }
        cout<<get(j, i);
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
      for(int i=0;i<size;i++){
          for(int j=0;j<size;j++){
              res += iToStr(get(i, j));
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
          nPieces[eated_player-1]++;
      }
      int a = get(move.end.x, move.end.y);
      set(move.end.x, move.end.y, 0);
      set(move.init.x, move.init.y, a);
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

