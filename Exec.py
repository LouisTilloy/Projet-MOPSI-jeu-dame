from GameModel import *
from grid import *
from piece import *
import sys



test_1 = False
test_2 = False
test_3 = False
test_4 = False
test_5 = False
test_6 = False
test_7 = False
test_8 = True



#-------------
#Pour le debug
#-------------

if test_1:
    G = Grid(10)
    G.disp()

if test_2:
    P = Player("Joueur 1", 15)
    print(P.isIA)

if test_3:
    G = Game()
    G.disp()
    G.move([0,3], [1,4])
    G.disp()

if test_4:
    G = Game()
    G.disp()
    G.begin()

if test_5:
    G = Game()
    G.disp()
    answer = input("coords:  ").split(",")
    coords = [int(answer[0]), int(answer[1])]
    if G[coords] != None:
        G[coords].isLady = True
    G.disp()

if test_6:
    G = Game()
    G[[1,6]].isLady = True
    G.move([3,6], [4,5])
    G.move([4,3], [3,4])
    G.disp()
    G.begin()

#------------------
#Pour les commandes
#------------------

def getGrid(str_grid, G):
    p1 = piece.Piece(1, False)
    p2 = piece.Piece(2, False)
    for i in range(G.size):
        for j in range(G.size):
            value = int(str_grid[i+G.size*j])
            if value==0:
                G[i,j] = None
            else:
                if value%2==0:
                    G[i,j] = copy.deepcopy(p2)
                else:
                    G[i,j] = copy.deepcopy(p1)
                if value>2:
                    G[(i,j)].isLady = True


if test_7:
    G = Game()
    cmd = sys.argv
    if len(cmd) > 3:
        getGrid(cmd[len(cmd)-1], G)
    if len(cmd) > 1:
        if cmd[1] == "get_moves":
            mot = cmd[2]
            coords = (int(mot[0]), int(mot[2]))
            moves = G.availableMoves(coords)
            print(moves)
        elif cmd[1] == "get_movables":
            coords = G.movable(int(cmd[2]))
            print(coords)
        elif cmd[1] == "get_eaters":
            coords = G.allAvailableEaters(int(cmd[2]))
            print(coords)
        elif cmd[1] == "get_eats":
            mot = cmd[2]
            coords = (int(mot[0]), int(mot[2]))
            eats = G.availableEats(coords)
            print(eats[1])
        elif cmd[1] == "occupy":
            mot = cmd[2]
            coords = (int(mot[0]), int(mot[2]))
            if G[coords] == None:
                print(0)
            else:
                if G[coords].player==1:
                    res = 1
                else:
                    res = -1
                if G[coords].isLady:
                    res*=2
                print(res)
        elif cmd[1] == "get_grid":
            tab = []
            for i in range(G.size):
                for j in range(G.size):
                    if G[(i,j)] == None:
                        tab += [0]
                    else:
                        if G[(i,j)].player==1:
                            tab += [1]
                        else:
                            tab += [2]
                        if G[(i,j)].isLady:
                            tab[len(tab)-1]+=2
            print(tab)
        elif cmd[1] == "get_playables":
            player = int(cmd[2])
            coords = G.playables(player)
            print(coords)
        elif cmd[1] == "play":
            player = int(cmd[2])
            mot_1 = cmd[3]
            mot_2 = cmd[4]
            coords_init = (int(mot_1[0]), int(mot_1[1]))
            coords_end = (int(mot_2[0]), int(mot_2[1]))
            n1 = G.players[player].nPieces
            if G.canEat(player):
                G.eat(coords_init, coords_end)
                n2 = G.players[3-player].nPieces
                print(n1-n1)
            else:
                G.move(coords_init, coords_end)
                n2 = G.players[3-player].nPieces
                print(n1-n2)
        elif cmd[1] == "can_eat":
            print(G.canEat(int(cmd[2])))

if test_8:
    G = Game()
    C = (1,6)
    D = (0,5)
    G.move(C, D)
    G.IA_turn(1)
    G.disp()
