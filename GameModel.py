# -*- coding: utf-8 -*-
"""
Contains:
    - the class Player which contains basic information of a player
    - the class Game which heritates from grid,
and which allows to play the game
"""
import copy

import grid
import piece


def diag(coords_1, coords_2):
    if(abs(coords_1[0]-coords_2[0]) != abs(coords_1[1]-coords_2[1])):
        return []
    liste = []
    ex = 1
    ey = 1
    if(coords_1[0] > coords_2[0]):
        ex = -1
    if(coords_1[1] > coords_2[1]):
        ey = -1
    for i in range(1,abs(coords_1[0]-coords_2[0])):
        liste += [(coords_1[0]+ex*i, coords_1[1]+ey*i)]
    return liste

class Player:
    """ Contains the basic information of a player """
    def __init__(self, name, nPieces, number, isIA=False):
        """ Creates a player with a given name and
        a given number of pieces,
        also indicates if it is an IA or not """
        if (not isinstance(name, str)):
            print("Name has to be a string")
            name = str(name)
        if (not isinstance(nPieces, int)):
            raise TypeError("The number of pieces owned by a player must \
            be an integer")
        if (nPieces < 0):
            raise ValueError("The number of pieces owned by a player must \
            be positive or zero")
        self.name = name
        self.nPieces = nPieces
        self._isIA = isIA
        self.number = number

    @property
    def isIA(self):
        return self._isIA

    def __str__(self):
        return self.name

class Game(grid.Grid):
    """ Contains all the methods necessary to play the game between
    two human players """
    def __init__(self, name1="Jessy", name2="James", size=10):
        grid.Grid.__init__(self, size, )

        if (size != 10):
            print("/!\ The official french version of the")
            print("game is played on a grid with a size of 10")

        p2 = piece.Piece(2, False) # Black pieces / player 2
        p1 = piece.Piece(1, False) # White pieces / player 1
        self._nPieces = 0 # initial number of pieces per player
        for y in range(0,size//2-1):
            for x in range(0,size//2):
                self[(x*2 + (y+1)%2, y)] = copy.deepcopy(p2)
                self[(x*2 + y%2, size-1-y)] = copy.deepcopy(p1)
                self._nPieces += 1

        self.player1 = Player(name1, self._nPieces, 1)
        self.player2 = Player(name2, self._nPieces, 2)
        self.players = [self.player1, self.player2]
        self.currentPlayer = self.player1 # White always starts the game

    def winner(self):
        """ returns the number of the player who won the game if the
        game is over, else returns false """
        if self.player1.nPieces == 0:
            return 2
        if self.player2.nPieces == 0:
            return 1
        return False

    def move(self, coordsInit, coordsFinal):
        """ moves the piece with the coordinates coordsInit to the position
        with the coordinages coordsFinal """
        self[coordsFinal] = self[coordsInit]
        self[coordsInit] = None

    def eat(self, coordsInit, coordsEaten):
        """ moves the piece with the coordinates coordsInit to the position
        where it must be after eating the piece with the coordinates
        coordsEaten """
        if not self[coordsInit].isLady:
            dx, dy = 1, 1
            if coordsEaten[0] - coordsInit[0] < 0:
                dx = -1
            if coordsEaten[1] - coordsInit[1] < 0:
                dy = -1
            coordsFinal = [coordsEaten[0]+dx, coordsEaten[1]+dy]
            self.move(coordsInit, coordsFinal)
            self[coordsEaten] = None
            return coordsFinal
        for coord in coordsEaten:
            self[coord] = None


    def availableMoves(self, coords):
        """ returns the list of coordinates the piece with the given coords
        can go to without eating another piece.
        returns None if there is no piece with the given coordinates
        """
        # If the position is empty, it returns a None
        if (self[coords] == None):
            return None

        listCoords = [] # Contains the list of available moves
        x = coords[0] # position of the piece of which we search
        y = coords[1] # the available moves

        # If the piece isn't a Lady, it can go forward in diagonal
        # but never backward, except to eat an enemy piece.
        forward = 0 # defines the direction forward
        if self[coords].player == 1:
            forward = -1
        else:
            forward = 1
        if not self[coords].isLady:
            if(self[x-1,y+forward] == None):
                listCoords.append((x-1,y+forward))
            if(self[x+1,y+forward] == None):
                listCoords.append((x+1,y+forward))

        # If the piece is a Lady, it can go in diagonal in every
        # direction and with any range but not behind another piece.
        if self[coords].isLady:
            for [dx,dy] in [[1,1], [-1,-1], [-1,1], [1,-1]]:
                dx0, dy0 = dx, dy
                while self[(x+dx0,y+dy0)] == None:
                    listCoords.append((x+dx0, y+dy0))
                    dx0 += dx
                    dy0 += dy
        return listCoords


    def availableEats(self, coords):
        """ returns the list of coordinates that the piece with the given coords
        can eat.
        returns None if there is no piece with the given coordinates
        """
        if self[coords]==None:
            return []
        if self[coords].player != self.currentPlayer.number:
            return []
        liste = []
        x, y = coords[0], coords[1]

        if self[coords].isLady:
            print("Lady")
            M1 = self.size-1-max(x, y)
            x_ex1 = x + M1
            y_ex1 = y + M1
            M2 = min(x, y)
            x_ex2 = x - M2
            y_ex2 = y - M2
            M3 = min(x, self.size-1-y)
            x_ex3 = x - M3
            y_ex3 = y + M3
            M4 = min(self.size-1-x, y)
            x_ex4 = x + M4
            y_ex4 = x + M4
            extrem_coords = [(x_ex1, y_ex1), (x_ex2, y_ex2), (x_ex3, y_ex3), (x_ex4, y_ex4)]
            diags = []
            for i in range(4):
                diags += [diag(coords, extrem_coords[i])]
            eats = []
            for j in range(4):
                eat = []
                piece_to_eat = False
                for coord in diags[j]:
                    if self[coord] != None:
                        if self[coord].player == self.currentPlayer.number:
                            break
                        piece_to_eat = True
                        eat += [coord]
                    else:
                        if piece_to_eat:
                            eats += [eat]
            return eats
            
        tabExplore_1 = [(x-1, y-1), (x-1, y+1), (x+1, y-1), (x+1, y+1)]
        tabExplore_2 = [(x-2, y-2), (x-2, y+2), (x+2, y-2), (x+2, y+2)]
        for i in range(4):
            if self[tabExplore_1[i]]!=False and self[tabExplore_2[i]] !=False and self[tabExplore_1[i]]!=None:
                if ((self[tabExplore_1[i]]).player==3-self.currentPlayer.number and self[tabExplore_2[i]] == None):
                    liste += [tabExplore_1[i]]
        return liste

    def allAvailableEaters(self, nPlayer):
        """ returns the list of all the coordinates where there is a
        piece belonging to the player number nPlayer that can eat
        another piece """
        liste = []
        for i in range(self.size):
            for j in range(self.size):
                if self[[i, j]] != None:
                    piece = self[[i,j]]
                    if(piece.player == nPlayer):
                        if self.availableEats((i, j)) != []:
                            liste += [(i, j)]
        return liste
        
    def allAvailableMovers(self, nPlayer):
        """ returns the list of all the coordinates where there is a 
        piece belonging to the player number nPlayer that can eat
        another piece """
        liste = []
        for i in range(self.size):
            for j in range(self.size):
                if self[[i, j]] != None:
                    piece = self[(i,j)]
                    if(piece.player == nPlayer):
                        if self.availableMoves((i, j)) != []:
                            liste += [(i, j)]
        return liste

    def canEat(self, nPlayer):
        """ returns a boolean that says if a player can eat during his
        turn
        NB : In the rules of the checkers (jeu de dame), you have to eat
        if you can """
        return self.allAvailableEaters(nPlayer) != []

    def askMove(self, nPlayer):
        """asks what move the current player wants to play at every step (if not IA)
        and returns the coordinates"""
        ask = True
        coords = None
        available_moves = None
        while ask:
            answer = input("Piece to move:   ").split(',')
            assert(len(answer)==2)
            coords = [int(answer[0]), int(answer[1])]
            if self[coords] != None:
                available_moves = self.availableMoves(coords)
                if self[coords].player==nPlayer and available_moves != [] and available_moves != None:
                    ask = False
        print("\n Available moves:  ", available_moves)
        ask = True
        final_coords = [-1,-1]
        while ask:
            answer = input("Move to:   ").split(',')
            assert(len(answer)==2)
            final_coords = [int(answer[0]), int(answer[1])]
            if final_coords in available_moves:
                ask = False
        return (coords, final_coords)

    def askEat(self, nPlayer):
        eaters = self.allAvailableEaters(nPlayer)
        print("Pieces that can eat: ", eaters)
        ask = True
        coords = None
        while ask:
            answer = input("Piece to move:   ").split(',')
            assert(len(answer)==2)
            coords = [int(answer[0]), int(answer[1])]
            if coords in eaters:
                ask = False
        eats = self.availableEats(coords)
        print("Pieces you can eat:  ", eats)
        ask = True
        coords_eaten = None
        while ask:
            answer = input("Piece to eat:   ").split(',')
            assert(len(answer)==2)
            coords_eaten = [int(answer[0]), int(answer[1])]
            if coords_eaten in eats:
                ask = False
        return [coords, coords_eaten]

    def changePlayer(self):
        self.currentPlayer = self.players[2-self.currentPlayer.number]

    def checkLady(self, nPlayer):
        """Changes the right pieces into ladies after each turn"""
        line = 0
        if nPlayer == 2:
            line = self.size-1
        for i in range(self.size):
            if self[i, line] != None:
                if self[i, line].player == nPlayer and self[i, line].isLady == False:
                    self[i, line].isLady = True
    def begin(self):
        """Starts the loop of the game."""
        while self.winner()==False:
            nPlayer = self.currentPlayer.number
            print("\n"+self.currentPlayer.name+"'s turn:\n")
            if self.canEat(nPlayer):
                print("eating...")
                eat = self.askEat(nPlayer)
                final_coords = self.eat(eat[0], eat[1])
                if self.availableEats(final_coords) == []:
                    self.changePlayer()
            else:
                move = self.askMove(nPlayer)
                self.move(move[0], move[1])
                self.changePlayer()
            self.checkLady(nPlayer)
            self.disp()

        print("\n-------------\n", "   END", "\n------------\n")
