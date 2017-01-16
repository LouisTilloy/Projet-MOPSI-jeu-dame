#include "tools.h"

vector<Coord> diag(Coord init, Coord end){
    vector<Coord> diagonale;
    diagonale.clear();
    if(init.x<0 || init.y<0 || end.x<0 || end.y<0){
        return diagonale;
    }
    if(abs(init.x-end.x) != abs(init.y-end.y)){
        return diagonale;
    }
    int dx = 1;
    int dy = 1;
    if(init.x > end.x){
        dx = -1;
    }
    if(init.y > end.y){
        dy = -1;
    }
    for(int i=1;i<abs(init.x-end.y);i++){
        Coord C(init.x+i*dx, init.y+i*dy);
        diagonale.push_back(C);
    }
    return diagonale;
}


bool canEat(int player, Grid G){
    string new_action = "can_eat " + iToStr(player) + " " + G.putStr();
    string str_bool = action(new_action);
    return (str_bool == "True");
}


Move min_max(Grid& G, int player, int depth){
    Move best_move;
    if(depth==0 || G.isEnded()){
        best_move.setPoints(G.points(player));
        return best_move;
    }
    int max_move = -50;
    vector<Coord> playable = getPlayables(player, G);
    for(int i=0;i<playable.size();i++){
        vector<Coord> plays = possiblePlays(player, playable[i], G);
        for(int j=0;j<plays.size();j++){
            Move move(playable[i], plays[j]);
            G.play(player, move);
            int mult = 1;
            if(canEat(player, G)){
                move = min_max(G, player, depth-1);
            }else{
                move = min_max(G, 3-player, depth-1);
                mult = -1;
            }
            G.go_back();
            int points = move.getPoints();
            move.setPoints(mult*points);
            if(move.getPoints()>max_move){
                max_move = move.getPoints();
                best_move.init = playable[i];
                best_move.end = plays[j];
            }
        }
    }
    return best_move;
}


vector<Coord> possiblePlays(int player, Coord start, Grid G){
    if(canEat(player, G)){
        return getEats(start, G);
    }else{
        vector<Move> moves = getMoves(start, G);
        vector<Coord> coords;
        coords.clear();
        for(int i=0;i<moves.size();i++){
            coords.push_back(moves[i].end);
        }
        return coords;
    }
}

string iToStr(int a){
  ostringstream oss;
  oss << a;
  return oss.str();
}

int strToInt(string s){
  istringstream iss(s);
  int res;
  iss >> res;
  return res;
}

int occupy_type(Coord C){
  string new_action = "occupy " + iToStr(C.y) + "," + iToStr(C.x);
  string str_int = action(new_action);
  return strToInt(str_int);
}


vector<Coord> getMovables(int player, Grid G){
  string new_action = "get_movables " + iToStr(player) + " " + G.putStr();
  string str_coords = action(new_action);
  int N = str_coords.size();
  vector<Coord> coords;
  int p = 0;
  int value;
  int coo_x, coo_y;
  for(int i=0;i<N;i++){
    value = str_coords[i] - '0';
    if(value>=0 && value<10){
      if(p%2==0){
        coo_x = str_coords[i] - '0';
      }else{
        coo_y = str_coords[i] - '0';
        Coord new_coo(coo_y, coo_x);
        coords.push_back(new_coo);
      }
      p++;
    }
  }
  return coords;
}

vector<Move> getMoves(Coord start, Grid G){
  string new_action = "get_moves "+ iToStr(start.y)+","+iToStr(start.x) + " " + G.putStr();
  string str_coords = action(new_action);
  int N = str_coords.size();
  vector<Move> moves;
  moves.clear();
  int p = 0;
  Coord end;
  int value;
  for(int i=0;i<N;i++){
    value = str_coords[i] - '0';
    if(value>=0 && value<10){
      if(p%2==0){
        end.y = str_coords[i] - '0';
      }else{
        end.x = str_coords[i] - '0';
        Move new_move(start, end);
        moves.push_back(new_move);
      }
      p++;
    }
  }
  return moves;
}

vector<Coord> getPlayables(int player, Grid G){
    string new_action = "get_playables " + iToStr(player) +" " + G.putStr();
    string str_coords = action(new_action);
    vector<Coord> coords;
    int p = 0;
    int N = str_coords.size();
    int value;
    int coo_x, coo_y;
    for(int i=0;i<N;i++){
        value = str_coords[i] - '0';
        if(value>=0 && value<10){
            if(p%2==0){
                coo_x = value;
            }else{
                coo_y = value;
                Coord new_coo(coo_y, coo_x);
                coords.push_back(new_coo);
            }
            p++;
        }
    }
    return coords;
}

vector<Coord> getEaters(int player, Grid G){
  string new_action = "get_eaters " + iToStr(player) + " " + G.putStr();
  string str_coords = action(new_action);
  int N = str_coords.size();
  vector<Coord> coords;
  int p = 0;
  int value;
  int coo_x, coo_y;
  for(int i=0;i<N;i++){
    value = str_coords[i] - '0';
    if(value>=0 && value<10){
      if(p%2==0){
        coo_x = value;
      }else{
        coo_y = value;
        Coord new_coo(coo_y, coo_x);
        coords.push_back(new_coo);
      }
      p++;
    }
  }
  return coords;
}



vector<Coord> getEats(Coord start, Grid G){
  string new_action = "get_eats " + iToStr(start.y) + "," + iToStr(start.x) + " " + G.putStr();
  string str_coords = action(new_action);
  int N = str_coords.size();
  vector<Coord> coords;
  int p = 0;
  int value;
  int coo_x, coo_y;
  for(int i=0;i<N;i++){
    value = str_coords[i] - '0';
    if(value>=0 && value<10){
      if(p%2==0){
        coo_x = value;
      }else{
        coo_y = value;
        Coord new_coo(coo_y, coo_x);
        coords.push_back(new_coo);
      }
      p++;
    }
  }
  return coords;
}


vector<int> getGrid(){
    string new_action = "get_grid";
    string str_tab = action(new_action);
    vector<int> tab;
    int value;
    int N = str_tab.size();
    for(int i=0;i<N;i++){
        value = str_tab[i] - '0';
        if(value>=0 && value<10){
            int a = value;
            tab.push_back(a);
        }
    }
    return tab;
}




int play(int player, Move move, Grid& G){
    string str_coo_init = iToStr(move.init.x) + "," +iToStr(move.init.y);
    string str_coo_end = iToStr(move.end.x) + "," + iToStr(move.end.y);
    string new_action = "play " + iToStr(player) + " " + str_coo_init + " " + str_coo_end + G.putStr();
    string str_int = action(new_action);
    int res = strToInt(str_int);
    return res;
}



string exec(const char* cmd){
  char buffer[128];
  string result = "";
  FILE* pipe = _popen(cmd, "r");
  if(!pipe){
    throw runtime_error("popen() failed!");
  }
  try{
    while(!feof(pipe)){
      if(fgets(buffer, 128, pipe) != NULL){
        result += buffer;
      }
    }
  }catch(...){
    _pclose(pipe);
    throw;
  }
  _pclose(pipe);
  return result;
}


string action(string actionName){
  string request = "python Exec.py " + actionName;
  string res = exec(request.c_str());
  return res;
}









