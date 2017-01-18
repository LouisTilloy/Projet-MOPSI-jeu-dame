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


class Coord{
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

vector<Coord> diag_large(Coord init, Coord end);
vector<Coord> diag(Coord init, Coord end);


class Move{
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



int occupy_type(Coord C);
vector<int> getGrid();


class Grid{
  int* tab;
  int size;
  bool ended;
  int nPieces[2];
  vector<Move> move_seq;
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
    if(get(i, j)==1 || get(i, j)==3){
        return 1;
    }else if(get(i, j)==2 || get(i, j)==4){
        return 2;
    }
    return 0;
  }
  int getPlayer(Coord C){
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
    if(i<0 || j<0 || i>=size || j>=size){
        return 5;
    }
    return tab[i*size+j];
  }
  int get(Coord C){
    return get(C.x, C.y);
  }
  void copy(){
    vector<int> tab_grid = getGrid();
    vect_copy(tab_grid);
  }
  void disp(){
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
      string res = "";
      for(int i=0;i<size;i++){
          for(int j=0;j<size;j++){
              res += iToStr(get(i, j));
          }
      }
      return res;
  }
  bool isEnded(){
      return (nPieces[0]<=0 || nPieces[1]<=0);
  }
  int points(int player){
      return nPieces[player-1] - nPieces[2-player];
  }

  vector<Coord> eated(int player, Move move){
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
  vector<Coord> availableMoves(Coord start);
  vector<Coord> movables(int player);
  vector<Coord> availableEats(Coord start);
  vector<Coord> availableEaters(int player);
  bool canEat(int player);
  vector<Coord> playables(int player);
  vector<Coord> availablePlays(Coord start);
  Move minMax(int player, int depth);
};

string action(string actionName);
vector<Move> getMoves(Coord start, Grid G);
string exec(const char* cmd);
vector<Coord> getMovables(int player, Grid G);
vector<Coord> getEaters(int player, Grid G);
vector<Coord> getEats(Coord start, Grid G);
Move min_max(Grid& G, int player, int depth);
bool canEat(int player, Grid G);
vector<Coord> getPlayables(int player, Grid G);
int play(int player, Move move, Grid G);
vector<Coord> possiblePlays(int player, Coord start, Grid G);
void send(Move move);
void send(vector<Coord> eats);

