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

class Player:
    """ Contains the basic information of a player """
    def __init__(self, name, nPieces, isIA=False):
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
        
        self.player1 = Player(name1, self._nPieces)
        self.player2 = Player(name2, self._nPieces)
        self.players = [self.player1, self.player2]
        self.currentPlayer = self.player1 # White always starts the game

    def winner(self):
        """ returns the number of the player who won the game if the 
        game is over, else returns false """
        if self._n1Pieces == 0:
            return 2
        if self._n2Pieces == 0:
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
        pass

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
                listCoords.append([x-1,y+forward])
            if(self[x+1,y+forward] == None):
                listCoords.append([x+1,y+forward])
                
        # If the piece is a Lady, it can go in diagonal in every
        # direction and with any range but not behind another piece.
        if self[coords].isLady:
            for [dx,dy] in [[1,1], [-1,-1], [-1,1], [1,-1]]:
                dx0, dy0 = dx, dy
                while self[(x+dx0,y+dy0)] == None:
                    listCoords.append([x+dx0, y+dy0])
                    dx0 += dx
                    dy0 += dy
        return listCoords
                

    def availableEats(self, coords):
        """ returns the list of coordinates that the piece with the given coords
        can eat. 
        returns None if there is no piece with the given coordinates
        """
        pass
    
    def allAvailableEaters(self, nPlayer):
        """ returns the list of all the coordinates where there is a 
        piece belonging to the player number nPlayer that can eat
        another piece """
        pass
    
    def canEat(self, nPlayer):
        """ returns a boolean that says if a player can eat during his
        turn 
        NB : In the rules of the checkers (jeu de dame), you have to eat
        if you can """
        pass