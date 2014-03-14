#!/usr/bin/env python3
# -*- coding: utf-8 -*-
 
"""The user interface for our app"""

import os,sys,time

# Import Qt modules
from PyQt4 import QtCore,QtGui
#from PyQt4 import QtOpenGL

# Import the compiled UI module
from random import randint, shuffle

# class LinkGraphicsItem(QtGui.QGraphicsLineItem):
#     def __init__(self, tail, head, parent=None, scene=None):
#         super().__init__(QtCore.QLineF(tail.pos(), head.pos()), parent=parent, scene=scene)
#         self.setPen(QtCore.Qt.white)
#         self.tail = tail
#         self.head = head
        

# class TerminalGraphicsItem(QtGui.QGraphicsRectItem):
#     def __init__(self, x, y, parent=None, scene=None):
#         super().__init__(-10, -10, 20, 20, parent=parent, scene=scene)
#         self.tempLine = QtGui.QGraphicsLineItem(0, 0, 0, 0, self, scene)
#         self.tempLine.setPen(QtCore.Qt.white)
#         self.tempLine.hide()
#         self.setPos(x,y)

#     def sceneEvent(self, event):
#         if event.type() == QtCore.QEvent.GraphicsSceneMousePress:
#             line = QtCore.QLineF(QtCore.QPointF(0,0), event.pos())
#             self.tempLine.setLine(line)
#             self.tempLine.show()
#             return True

#         if event.type() == QtCore.QEvent.GraphicsSceneMouseMove:
#             line = QtCore.QLineF(QtCore.QPointF(0,0), event.pos())
#             self.tempLine.setLine(line)
#             return True

#         if event.type() == QtCore.QEvent.GraphicsSceneMouseRelease:
#             self.tempLine.hide()
#             return True

#         return super().sceneEvent(event)

class NodeGraphicsItem(QtGui.QGraphicsEllipseItem):
    def __init__(self, parent=None, scene=None, *args, **kwargs):
        super().__init__(100, 100, 15, 15, parent, scene, *args, **kwargs)

        self.setFlag(QtGui.QGraphicsItem.ItemIsMovable, True)
        self.setBrush(QtGui.QBrush(QtGui.QColor("Blue")))

# Create a class for our main window
class Main(QtGui.QWidget):
    def __init__(self):
        QtGui.QWidget.__init__(self)

        self.scene=QtGui.QGraphicsScene()
        self.scene.setSceneRect(0,0,800,600)

        self.layout=QtGui.QBoxLayout(QtGui.QBoxLayout.LeftToRight, self)

        self.view=QtGui.QGraphicsView(self)
        self.view.setScene(self.scene)
        self.view.setBackgroundBrush(QtGui.QBrush(QtGui.QColor("White")))

        self.layout.addWidget(self.view)

        self.populate()

    def populate(self):
        self.box1 = NodeGraphicsItem(scene=self.scene)
        self.box1.setPos(50,50)

        self.box2 = NodeGraphicsItem(scene=self.scene)
        self.box2.setPos(300,50)

def main():
    app = QtGui.QApplication(sys.argv)
    window=Main()
    window.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
