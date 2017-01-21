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
    for(int i=1;i<abs(init.x-end.x);i++){
        Coord C(init.x+i*dx, init.y+i*dy);
        diagonale.push_back(C);
    }
    return diagonale;
}


vector<Coord> diag_large(Coord init, Coord end){
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
    for(int i=1;i<abs(init.x-end.x)+1;i++){
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


//Methodes

vector<Coord> Grid::availableMoves(Coord start){
    vector<Coord> res;
    res.clear();
    int value = (*this)[start];
    if(value == 0){
        return res;
    }
    int x = start.x;
    int y = start.y;
    int forward = 1;
    if(value%2==1){
        forward = -1;
    }
    if(value<3){
        if(get(x-1, y+forward)==0){
            Coord C1(x-1, y+forward);
            res.push_back(C1);
        }
        if(get(x+1, y+forward)==0){
            Coord C2(x+1, y+forward);
            res.push_back(C2);
        }
    }else{
        for(int i=-1;i<2;i+=2){
            for(int j=-1;j<2;j+=2){
                int p=1;
                while(get(x+p*i, y+p*j)==0){
                    Coord C(x+p*i, y+p*j);
                    res.push_back(C);
                    p++;
                }
            }
        }
    }
    return res;
}

vector<Coord> Grid::movables(int player){
    vector<Coord> res;
    res.clear();
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(getPlayer(i,j)==player){
                Coord C(i, j);
                if(availableMoves(C).size()>0){
                    res.push_back(C);
                }
            }
        }
    }
    return res;
}

vector<Coord> Grid::availableEats(Coord start){
    vector<Coord> moves;
    moves.clear();
    int value = (*this)[start];
    int x = start.x;
    int y = start.y;
    if(value==3 || value==4){
        int M1 = size-1-max(x, y);
        int x_ex1 = x + M1;
        int y_ex1 = y + M1;
        int M2 = min(x, y);
        int x_ex2 = x - M2;
        int y_ex2 = y - M2;
        int M3 = min(x, size-1-y);
        int x_ex3 = x - M3;
        int y_ex3 = y + M3;
        int M4 = min(size-1-x, y);
        int x_ex4 = x + M4;
        int y_ex4 = y - M4;
        Coord C1(x_ex1, y_ex1);
        Coord C2(x_ex2, y_ex2);
        Coord C3(x_ex3, y_ex3);
        Coord C4(x_ex4, y_ex4);
        Coord extrem_coords[4] = {C1, C2, C3, C4};
        vector<Coord> diags[4];
        for(int i=0;i<4;i++){
            diags[i] = diag_large(start, extrem_coords[i]);
        }
        bool can_eat;
        for(int i=0;i<4;i++){
            can_eat = false;
            for(int j=0;j<diags[i].size();j++){
                if(getPlayer(diags[i][j])==getPlayer(start)){
                    break;
                }else if(getPlayer(diags[i][j])==3-getPlayer(start)){
                    can_eat = true;
                }else if(get(diags[i][j])==0 && can_eat){
                    moves.push_back(diags[i][j]);
                }
            }
        }
        return moves;
    }
    Coord D1(x-2, y-2);
    Coord D2(x-2, y+2);
    Coord D3(x+2, y-2);
    Coord D4(x+2, y+2);
    Coord move_test[4] = {D1, D2, D3, D4};
    int player = getPlayer(start);
    for(int i=0;i<4;i++){
        if(get(move_test[i])==0){
            Move M(start, move_test[i]);
            if(eated(player, M).size()>0){
                moves.push_back(move_test[i]);
            }
        }
    }
    return moves;
}

vector<Coord> Grid::availableEaters(int player){
    vector<Coord> res;
    res.clear();
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            Coord C(i, j);
            if(getPlayer(C)==player && availableEats(C).size()>0){
                res.push_back(C);
            }
        }
    }
    return res;
}

bool Grid::canEat(int player){
    return (availableEaters(player).size()>0);
}


vector<Coord> Grid::playables(int player){
    vector<Coord> res;
    res.clear();
    if(canEat(player)){
        return availableEaters(player);
    }
    return movables(player);
}


vector<Coord> Grid::availablePlays(Coord start){
    int player = getPlayer(start);
    if(canEat(player)){
        return availableEats(start);
    }
    return availableMoves(start);
}


Move Grid::minMax(int player, int depth){
    Move best_move;
    if(depth==0 || isEnded()){
        best_move.setPoints(points(player));
        return best_move;
    }
    int max_move = -50;
    vector<Coord> playable = playables(player);
    for(int i=0;i<playable.size();i++){
        vector<Coord> plays = availablePlays(playable[i]);
        for(int j=0;j<plays.size();j++){
            bool play_again = (availableEats(playable[i]).size()>0);
            Move move(playable[i], plays[j]);
            play(player, move);
            play_again = (play_again && availableEats(plays[j]).size()>0);
            int mult = 1;
            if(play_again){
                move = minMax(player, depth-1);
            }else{
                move = minMax(3-player, depth-1);
                mult = -1;
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



void send(Move move){
    cout<<move.init.x<<","<<move.init.y<<";"<<move.end.x<<","<<move.end.y<<endl;
}

void send(vector<Coord> eats){
    for(int i=0;i<eats.size();i++){
        if(i>1){
            cout<<";";
        }
        cout<<eats[i].x<<","<<eats[i].y;
    }
    cout<<endl;
}






