# -*- coding: utf-8 -*-
""" Contains the class 'Grid' of the checkers game """

class NotInteger(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return "{} n'est pas un entier !".format(self.value)

class NotPositive (Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return "{} est négatif !".format(self.value)

class Grid:
    """ Code the grid of the checkers game with a list """
    def __init__(self, size):
        """ Create a square grid with the given size and
        filled with None """
        if (not isinstance(size, int)):
            raise NotInteger(size)
        if (size <= 0):
            raise NotPositive(size)
        self._size = size
        self._tab = [[None]*self.size for i in range(0,self.size)]

    @property
    def size(self):
        """ returns the size of the grid """
        return self._size

    def __getitem__(self, coords):
        """ Access the value of the given coordinates
        with the syntax: self[(x, y)] or self[[x, y]]
        returns False if the coordinates are not in the grid
        """
        x = coords[0]
        y = coords[1]
        if 0 <= x < self._size and 0 <= y < self._size:
            return self._tab[x][y]
        else:
            return False

    def __setitem__(self, coords, value):
        """ Set the value of the square with the given coordinates
        with the syntax: self[(x, y)] = value or self[[x, y]] = value
        """
        x = coords[0]
        y = coords[1]
        self._tab[x][y] = value

    def disp(self):
        """ Display a simple version of the grid,
        this is mostly to debug """
        subSize = len(str(self._tab[0][0])) + 4 # Size of 1 subSquare of the grid
        print("-"*(self._size*(subSize + 1) + 1))
        for y in range(0,self._size):
            line = "|"
            for x in range(0,self._size):
                line += "{0:^{width}}|".format(str(self._tab[x][y]) \
                                    , width = subSize)
            print(line)
            print("-"*(self._size*(subSize + 1) + 1))
