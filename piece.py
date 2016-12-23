# -*- coding: utf-8 -*-
""" 
Contains the class Piece which code the pieces of the game    
"""

class Piece:
    """ Code a piece of the game, it has two fields:
        _player which is an integer (1 or 2) representing 
                                                the player owning the piece
        _isLady which codes the nature of the piece (normal or Lady)
    """
    def __init__(self, player, isLady):
        """ Creates a piece owned by player,
        which is a lady if isLady is true """
        if not isinstance(player, int):
            raise TypeError("player should be an integer in {1,2}")
        if player not in [1,2]:
            raise ValueError("player should be equal to 1 or 2".format(player))
        if not isinstance(isLady, bool):
            raise TypeError("isLady should be a boolean")
        self._player = player # 1 for player 1, 2 for player 2
        self._isLady = isLady
        
    @property
    def player(self):
        """ Access the player that owns the piece """
        return self._player
        
    @property
    def isLady(self):
        """ returns a boolean that indicates if the piece is a Lady """
        return self._isLady
        
    @isLady.setter
    def isLady(self, value):
        """ sets the nature of the piece (normal or Lady) """
        if not isinstance(value, bool):
            raise TypeError("value should be a boolean")
        self._isLady = value
        
    def __str__(self):
        """ only for debugging,
        returns the player owning the piece as well as if it is a Lady
        with this syntax: 
            (player owning the piece, type of the piece)
        """
        typePiece = ""
        if (self._isLady):
            typePiece = "L" # The piece is a Lady
        else:
            typePiece = "N" # The piece is a normal piece
        return "({0}, {1})".format(self._player, typePiece)
        
        