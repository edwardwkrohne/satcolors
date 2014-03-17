#!/usr/bin/env python3
# -*- coding: utf-8 -*-
 
"""The user interface for our app"""

import os,sys,time

# Import Qt modules
from PyQt4 import QtCore,QtGui

class ColorAction(QtGui.QWidgetAction):
    colorSelected = QtCore.pyqtSignal(QtGui.QColor)

    def __init__(self, parent):
        QtGui.QWidgetAction.__init__(self, parent)
        widget = QtGui.QWidget(parent)
        layout = QtGui.QGridLayout(widget)
        layout.setSpacing(0)
        layout.setContentsMargins(2, 2, 2, 2)
        palette = self.palette()
        count = len(palette)
        rows = 6
        for row in range(rows):
            for column in range(count // rows):
                color = palette.pop()
                button = QtGui.QToolButton(widget)
                button.setAutoRaise(True)
                button.clicked[()].connect(
                    lambda color=color: self.handleButton(color))
                pixmap = QtGui.QPixmap(16, 16)
                pixmap.fill(color)
                button.setIcon(QtGui.QIcon(pixmap))
                layout.addWidget(button, row, column)
        self.setDefaultWidget(widget)

    def handleButton(self, color):
        self.parent().hide()
        self.colorSelected.emit(color)

    def palette(self):
        palette = []
        for b in range(6):
            for g in range(6):
                for r in range(6):
                    palette.append(QtGui.QColor(
                        r * 255 // 5, g * 255 // 5, b * 255 // 5))
        return palette

class ColorMenu(QtGui.QMenu):
    def __init__(self, parent):
        QtGui.QMenu.__init__(self, parent)
        self.colorAction = ColorAction(self)
        self.colorAction.colorSelected.connect(self.handleColorSelected)
        self.addAction(self.colorAction)
        self.addSeparator()
        self.addAction('Custom Color...')

    def handleColorSelected(self, color):
        print(color.name())

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


class EdgeGraphicsItem(QtGui.QGraphicsLineItem):
    def __init__(self, tail, head, parent=None, scene=None):
        super().__init__(QtCore.QLineF(tail.pos(), head.pos()), parent=parent, scene=scene)
        self.setZValue(-1)
        self.setPen(QtCore.Qt.black)
        self.tail = tail
        self.head = head

    def updatePositions(self):
        self.setLine(QtCore.QLineF(self.tail.pos(), self.head.pos()))

class NodeGraphicsItem(QtGui.QGraphicsEllipseItem):
    def __init__(self, *args, **kwargs):
        super().__init__(-10, -10, 20, 20, *args, **kwargs)

        self.setFlag(QtGui.QGraphicsItem.ItemIsMovable, True)
        self.setFlag(QtGui.QGraphicsItem.ItemIsSelectable, True)
        self.setFlag(QtGui.QGraphicsItem.ItemSendsGeometryChanges, True)
        self.setBrush(QtGui.QBrush(QtGui.QColor("Blue")))

        self.edges = set()

    def getEdges(self):
        return self.edges

    def addEdge(self, edge):
        self.edges.add(edge)

    def removeEdge(self, edge):
        self.edges.discard(edge)

    def itemChange(self, change, value):
        if change == QtGui.QGraphicsItem.ItemPositionHasChanged:
            for edge in self.edges:
                edge.updatePositions()
        return super().itemChange(change, value)
    

class GraphView(QtGui.QGraphicsView):
    def __init(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.setBackgroundBrush(QtGui.QBrush(QtGui.QColor("White")))

        # Start in "add" mode
        self.setModeAdd()

    def mousePressEvent(self, event):
        if self.isModeAdd():
            # Just add a new node
            point = self.mapToScene(event.pos())
            node = NodeGraphicsItem(scene=self.scene())
            node.setPos(point)

        elif self.isModeDelete():
            # Find the appropriate node and delete it (if it's indeed a node)
            item = self.itemAt(event.pos())
            if isinstance(item, NodeGraphicsItem):
                node = item
                for edge in list(node.getEdges()):
                    edge.head.removeEdge(edge)
                    edge.tail.removeEdge(edge)
                    self.scene().removeItem(edge)
                self.scene().removeItem(node)

        elif self.isModeConnect():
            item = self.itemAt(event.pos())
            # Nothing to do if the user didn't click on a node
            if not isinstance(item, NodeGraphicsItem):
                return
                
            # Toggle the selection on the given node
            item.setSelected(not item.isSelected())

            # Get the selected items
            nodes = self.scene().selectedItems()

            if ( len(nodes) > 2):
                raise Exception("Too many nodes selected for creating a link: " + str(nodes))

            elif ( len(nodes) == 2 ):
                edgesInCommon = nodes[0].getEdges().intersection(nodes[1].getEdges())
                if ( len(edgesInCommon) == 0 ):
                    # Add a new edge
                    edge = EdgeGraphicsItem(nodes[0], nodes[1], scene=self.scene())
                    nodes[0].addEdge(edge)
                    nodes[1].addEdge(edge)
                else:
                    # Delete the old edge (there should only be one, but why not loop?)
                    for edge in edgesInCommon:
                        nodes[0].removeEdge(edge)
                        nodes[1].removeEdge(edge)
                        self.scene().removeItem(edge)
                
                self.scene().clearSelection()

        elif self.isModeMove():
            # Forward the mouse click to individual items
            super().mousePressEvent(event)

        else:
            raise Exception("Unknown mode: " + str(self.mode))


    def setModeAdd(self, isSelected):
        if ( isSelected ):
            self.mode = 'add'

    def isModeAdd(self):
        return self.mode == 'add'

    def setModeConnect(self, isSelected):
        self.scene().clearSelection()
        if ( isSelected ):
            self.mode = 'connect'

    def isModeConnect(self):
        return self.mode == 'connect'

    def setModeDelete(self, isSelected):
        if ( isSelected ):
            self.mode = 'delete'

    def isModeDelete(self):
        return self.mode == 'delete'

    def setModeMove(self, isSelected):
        if ( isSelected ):
            self.mode = 'move'

    def isModeMove(self):
        return self.mode == 'move'

    def setModePaint(self, isSelected):
        if ( isSelected ):
            self.mode = 'paint'

    def isModePaint(self):
        return self.mode == 'paint'

    def setModeColorPick(self, isSelected):
        if ( isSelected ):
            self.mode = 'colorPick'

    def isModeColorPick(self):
        return self.mode == 'colorPick'


# Create a class for our main window
class Main(QtGui.QMainWindow):
    def __init__(self):
        super().__init__()

        self.scene=QtGui.QGraphicsScene()
        self.scene.setSceneRect(0,0,800,600)

        self.view=GraphView(self)
        self.view.setScene(self.scene)

        self.setCentralWidget(self.view)

        self.setupToolbar()

        self.show()

 
    def setupToolbar(self):
        newAction = QtGui.QAction(QtGui.QIcon('new.png'), 'New Graph', self)
        openAction = QtGui.QAction(QtGui.QIcon('open.png'), 'Open Graph', self)
        saveAction = QtGui.QAction(QtGui.QIcon('save.png'), 'Save Graph', self)

        addAction = QtGui.QAction(QtGui.QIcon('add.png'), '&Add Node (A)', self)
        addAction.setShortcut('A')
        addAction.toggled.connect(self.view.setModeAdd)

        deleteAction = QtGui.QAction(QtGui.QIcon('delete.png'), '&Delete Node (D)', self)
        deleteAction.setShortcut('D')
        deleteAction.toggled.connect(self.view.setModeDelete)

        connectAction = QtGui.QAction(QtGui.QIcon('connect.png'), 'Toggle &Connection (C)', self)
        connectAction.setShortcut('C')
        connectAction.toggled.connect(self.view.setModeConnect)

        moveAction = QtGui.QAction(QtGui.QIcon('move.png'), 'Mo&ve Node (V)', self)
        moveAction.setShortcut('V')
        moveAction.toggled.connect(self.view.setModeMove)

        paintAction = QtGui.QAction(QtGui.QIcon(), 'Paint', self)
        paintAction.toggled.connect(self.view.setModePaint)

        paletteAction = QtGui.QAction(QtGui.QIcon('palette.png'), 'Palette', self)
        paletteAction.triggered.connect(self.popupColorMenu)

        colorPickAction = QtGui.QAction(QtGui.QIcon('color picker.png'), 'Pick Color from Node', self)
        colorPickAction.toggled.connect(self.view.setModeColorPick)

        exitAction = QtGui.QAction(QtGui.QIcon('exit.png'), '&Exit', self);
        exitAction.setShortcut('Ctrl+Q')
        exitAction.triggered.connect(QtGui.qApp.quit)

        self.toolbar = self.addToolBar('Toolbar')
        self.toolbar.addAction(newAction)
        self.toolbar.addAction(openAction)
        self.toolbar.addAction(saveAction)
        self.toolbar.addSeparator()
        self.toolbar.addAction(addAction)
        self.toolbar.addAction(deleteAction)
        self.toolbar.addAction(connectAction)
        self.toolbar.addAction(moveAction)
        self.toolbar.addAction(paintAction)
        self.toolbar.addAction(colorPickAction)
        self.toolbar.addAction(paletteAction)
        self.toolbar.addSeparator()
        self.toolbar.addAction(exitAction)

        editActionGroup = QtGui.QActionGroup(self)
        for action in [addAction, deleteAction, connectAction, moveAction, paintAction, colorPickAction]:
            editActionGroup.addAction(action)
            action.setCheckable(True)

        addAction.setChecked(True)

    def popupColorMenu(self, *args):
        menu = ColorMenu(self)
        screenPos = QtGui.QCursor.pos()
        menu.popup(screenPos)


def main():
    app = QtGui.QApplication(sys.argv)
    window=Main()
    window.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
