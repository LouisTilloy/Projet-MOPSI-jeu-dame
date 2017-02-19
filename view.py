import sys
from PyQt4 import QtGui, QtCore
import copy
import time

import GameModel


DEPTH = 7


class Square(QtGui.QPushButton):
    """ Button to represent the squares of the game """
    def __init__(self, x, y):
        """  Creates a square of the grid, whith the color coresponding to its
        position """
        super(Square, self).__init__()

        self.initUI(x, y)

    def initUI(self, x, y):
        """ Constructor """
        if (x+y)%2==1:
            self.setStyleSheet("QPushButton {background-color: black}"
                               "QPushButton {border: 0px}") # So it doesn't look
                                                            # like a button
        else:
            self.setStyleSheet("QPushButton {background-color: white}"
                               "QPushButton {border: 0px}")
        self.setFixedSize(80,80)

    def addPiece(self, nPlayer, isLady):
        """ Add a piece of the given player (a Lady if isLady = True) on the
        square """
        iconName = ""
        if nPlayer == 1:
            if isLady:
                iconName = "WhiteLady.png"
            else:
                iconName = "WhitePiece.png"
        else:
            if isLady:
                iconName = "BlackLady.png"
            else:
                iconName = "BlackPiece.png"

        pixmap = QtGui.QPixmap(iconName)
        icon = QtGui.QIcon(pixmap)
        self.setIcon(icon)
        self.setIconSize(pixmap.rect().size())

    def removePiece(self):
        """ Remove the piece of the square """
        pixmap = QtGui.QPixmap("")
        icon = QtGui.QIcon(pixmap)
        self.setIcon(icon)

    def availableSelect(self):
        """ Changes the color of the square when it is part of the squares
        that can be selected """
        self.setStyleSheet("QPushButton {background-color: rgb(20,100,20)}"
                               "QPushButton {border: 0px}")

    def select(self):
        """ Changes the color of the square when it is selected """
        self.setStyleSheet("QPushButton {background-color: blue}"
                               "QPushButton {border: 0px}")

    def unselect(self):
        """ Changes the color of the square back to its original one """
        # A white square will never be selected
        self.setStyleSheet("QPushButton {background-color: black}"
                               "QPushButton {border: 0px}")

    def gotoSelect(self):
        """ Changes the color of the square when it is part of potential
        destinations """
        pass

class Window(QtGui.QWidget):
    """ Main window in which we are going to put the game grid """
    def __init__(self, game):
        """ Constructeur, call the constructor of QWidget """
        super(Window, self).__init__()

        self.initUI(game)

    def initUI(self, game):
        """ Constructor, initializes the grid with the different pieces,
        """
        self.checkers = game # variable containing the game
        self.selectedPosition = None # position of the selected square or None
                                   # if there is no selected square
        self.selectedButton = None # button selected or None is there is no
                                   # button selected
        self.buttonList = GameModel.grid.Grid(game.size)
        self.replay = False # tells if a player is playing again to eat another
                            # piece

        grid = QtGui.QGridLayout()
        grid.setSpacing(0)

        # Creation of the buttons
        for x in range(0 ,self.checkers.size):
            for y in range(0, self.checkers.size):
                button = Square(x, y)
                square0 = self.checkers[(x,y)]
                if square0 != None:
                    button.addPiece(square0.player, square0.isLady)
                button.setObjectName(str((x, y)))
                button.clicked.connect(self.buttonClicked,)
                self.buttonList[(x,y)] = button
                grid.addWidget(button, y, x)

        self.setLayout(grid)
        self.show()


    def buttonClicked(self):
        """ Called whenever a button is pressed,
        the method does one turn of the game """
        print("A button has been pressed !")
        sender = self.sender()
        oldPosition = self.selectedPosition # refers to the position of the selected
                                            # piece
        position = (int(sender.objectName()[1]), int(sender.objectName()[4]))
        nPlayer = self.checkers.currentPlayer.number

        # If a square has already been selected
        print(self.selectedPosition)
        if oldPosition != None:
            #self.resetGraphicSelections()
            isLady = self.checkers[oldPosition].isLady
            # If you can eat, you have to
            if self.checkers.canEat(nPlayer):
                listMoves = self.checkers.availableEats(oldPosition)[1]
                allMoves = []
                for sublist in listMoves:
                    allMoves += sublist
                for move in allMoves:
                    self.buttonList[move].availableSelect()
                if position in allMoves:
                    eatenCoords = self.checkers.eat(oldPosition, position)
                    self.selectedButton.removePiece()
                    self.buttonList[eatenCoords].removePiece()
                    self.buttonList[position].addPiece(nPlayer, isLady)
                    print(self.checkers.availableEats(position))
                    print(self.checkers[position])
                    print(position)
                    if self.checkers.availableEats(position)[0] != []:
                        self.selectedButton.unselect()
                        self.selectedButton = self.buttonList[position]
                        self.selectedButton.select()
                        self.selectedPosition = copy.deepcopy(position)
                        self.replay = True
                    else:
                        self.checkers.changePlayer()
                        self.replay = False
                        self.resetGraphicSelections()
            # If you can't you have to move a piece
            else:
                allMoves = self.checkers.availableMoves(oldPosition)
                for move in allMoves:
                    self.buttonList[move].availableSelect()
                print(self.checkers.availableMoves(oldPosition))
                if position in allMoves:
                    print(2)
                    self.checkers.move(oldPosition, position)
                    self.selectedButton.removePiece()
                    self.buttonList[position].addPiece(nPlayer, isLady)
                    self.checkers.changePlayer()
                    self.resetGraphicSelections()
            # If the selected square cannot be reached with the piece, the player
            # has to start again the selection
            if (not self.replay):
                self.checkers.checkLady(self.checkers.currentPlayer.number)
                self.selectedButton = None
                self.selectedPosition = None
                self.resetGraphicSelections()
                self.graphicCheckLady(self.checkers.currentPlayer.number)
                self.graphicSelection(self.checkers.currentPlayer.number)

        # If no square has been selected
        elif position in self.checkers.playables(nPlayer):
            self.selectedPosition = copy.deepcopy(position)
            self.selectedButton = sender

        if self.selectedButton != None:
            self.resetGraphicSelections()
            self.selectedButton.select()
            listEatMoves = self.checkers.availableEats(self.selectedPosition)[1]
            if listEatMoves:
                for listMoves in listEatMoves:
                    for move in listMoves:
                        self.buttonList[move].availableSelect()
            else:
                for move in self.checkers.availableMoves(self.selectedPosition):
                    self.buttonList[move].availableSelect()
        self.graphicCheckLady(1)
        while self.checkers.currentPlayer.number == 2:
            self.resetGraphicSelections()
            move, eats = self.checkers.IA_turn(DEPTH)
            isLady = self.checkers[move[1]].isLady
            print(move[0], move[1])
            self.buttonList[move[0]].removePiece()
            self.buttonList[move[1]].addPiece(2, isLady)
            for eat in eats:
                self.buttonList[eat].removePiece()
            if not (self.checkers.canEat(2) and len(eats)>0):
                self.checkers.currentPlayer = self.checkers.player1
        self.graphicCheckLady(2)
        self.graphicSelection(1)
        return None

    def resetGraphicSelections(self):
        for x in range(0, self.checkers.size):
            for y in range(0, self.checkers.size):
                if (x+y)%2 == 1:
                    self.buttonList[(x,y)].unselect()

    def graphicSelection(self, nPlayer):
        for position in self.checkers.playables(nPlayer):
            self.buttonList[position].availableSelect()

    def graphicCheckLady(self, nPlayer):
        y = 0
        if nPlayer == 2:
            y = self.checkers.size-1
        for x in range(0, self.checkers.size):
            if self.checkers[(x,y)] != None and self.checkers[(x,y)].player == nPlayer:
                """ and self.checkers[(x,y)].isLady:"""
                self.checkers[(x, y)].isLady=True
                self.buttonList[(x,y)].addPiece(nPlayer, True)

def main():
    app = QtGui.QApplication(sys.argv)
    game = GameModel.Game()
    fenetre = Window(game)
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
