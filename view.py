import sys
from PyQt4 import QtGui, QtCore
import copy

import GameModel 

class Square(QtGui.QPushButton):
    """ Button to represent the squares of the game """
    def __init__(self, x, y):
        """  Creates a square of the grid, whith the color coresponding to its
        position """
        super(Square, self).__init__()
        
        self.initUI(x, y)
        
    def initUI(self, x, y):
        if (x+y)%2==1:
            self.setStyleSheet("QPushButton {background-color: black}"
                               "QPushButton {border: 0px}")
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
        
    def select(self):
        """ Changes the color of the square when it is selected """
        self.setStyleSheet("QPushButton {background-color: blue}"
                               "QPushButton {border: 0px}")
        
    def unselect(self):
        """ Changes the color of the square back to its original one """
        # A white square will never be selected
        self.setStyleSheet("QPushButton {background-color: black}"
                               "QPushButton {border: 0px}")
        
class Window(QtGui.QWidget):
    """ Fenêtre principale danslaquelle on va placer la grille de jeu  """
    def __init__(self, game):
        """ Constructeur, fait appel au constructeur de QMainWindow """
        super(Window, self).__init__()
        
        self.initUI(game)
        
    def initUI(self, game):
        self.checkers = game
        self.selectedPosition = None # position of the selected square or None
                                   # if there is no selected square
        self.selectedButton = None # button selected or None is there is no 
                                   # button selected
        self.buttonList = GameModel.grid.Grid(game.size)
        self.replay = False
        
        grid = QtGui.QGridLayout()
        grid.setSpacing(0)
        
        for x in range(0 ,self.checkers.size):
            for y in range(0, self.checkers.size):
                button = Square(x, y)
                square0 = self.checkers[(x,y)]
                if square0 != None:
                    button.addPiece(square0.player, square0.isLady)
                button.setObjectName(str((x, y)))
                button.clicked.connect(self.buttonClicked)
                self.buttonList[(x,y)] = button
                grid.addWidget(button, y, x)
        
        self.setLayout(grid)
        self.show()

    def buttonClicked(self):
        print("A button has been pressed !")
        sender = self.sender()
        oldPosition = self.selectedPosition
        position = (int(sender.objectName()[1]), int(sender.objectName()[4]))
        nPlayer = self.checkers.currentPlayer.number
        
        # If a square has already been selected
        print(self.selectedPosition)
        if oldPosition != None:
            isLady = self.checkers[oldPosition].isLady
            # If you can eat, you have to
            if self.checkers.canEat(nPlayer):
                if position in self.checkers.availableEats(oldPosition):
                    print(1)
                    finalPosition = self.checkers.eat(oldPosition, position)
                    self.selectedButton.removePiece()
                    self.buttonList[position].removePiece()
                    self.buttonList[finalPosition].addPiece(nPlayer, isLady)
                    if self.checkers.availableEats(finalPosition) != []:
                        self.selectedButton.unselect()
                        self.selectedButton = self.buttonList[finalPosition]
                        self.selectedButton.select()
                        self.selectedPosition = copy.deepcopy(finalPosition)
                        self.replay = True
                    else:
                        self.checkers.changePlayer()
                        self.replay = False
            # If you can't you have to move a piece
            else:
                print(self.checkers.availableMoves(oldPosition))
                if position in self.checkers.availableMoves(oldPosition):
                    print(2)
                    self.checkers.move(oldPosition, position)
                    self.selectedButton.removePiece()
                    self.buttonList[position].addPiece(nPlayer, isLady)
                    self.checkers.changePlayer()
            
            if (not self.replay):
                self.selectedButton.unselect()
                self.selectedButton = None
                self.selectedPosition = None
            
        # If no square has been selected
        else:
            if self.checkers[position] != None and \
                self.checkers[position].player == nPlayer and \
                (self.checkers.availableMoves(position) != [] or self.checkers.availableEats(position) != []):
                sender.select()
                self.selectedPosition = copy.deepcopy(position)
                self.selectedButton = sender
                if self.checkers.canEat(nPlayer):
                    print("yolo")
        return None
        
    

def main():
    app = QtGui.QApplication(sys.argv)
    game = GameModel.Game()
    fenetre = Window(game)
    sys.exit(app.exec_())
    
if __name__ == '__main__':
    main()

                