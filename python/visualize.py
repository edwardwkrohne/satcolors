#!/usr/bin/env python

from xml.dom import minidom
import numpy as np
import sys
import time
from PyQt4 import QtCore, QtGui, QtSvg
from PyQt4.QtGui import QMainWindow, QApplication
from PyQt4.QtCore import QTimer, QObject

def numIter(file, wait=False):
    # FIXME Some kind of file integrity check here.
    while True:
        line = file.readline();
        if not wait and line == "":
            return
        else:
            # Wait for another line
            while ( line  == "" ):
                time.sleep(1)
                line = file.readline();

        for word in line.strip().split():
            if ( word[0:2] == "0x" ):
                yield int(word, 16)
            elif ( word[0] == "0" ):
                yield int(word, 8)
            else:
                yield int(word, 10)

if ( len(sys.argv) < 3 ):
    print "Insufficient arguments."
    print "python", sys.argv[0], "<solution file> <palette file>"
    sys.exit(2)

# Grab the palette first
data = numIter(open(sys.argv[2], "r"))
palette = {}
try:
    while True:
        # FIXME: Do some kind of sanity check on this file
        index = data.next()
        color = data.next()
        palette[index] = QtGui.QColor.fromRgb(color)
except StopIteration:
    pass

# Now read in the solution while building a scene
data = numIter(open(sys.argv[1], "r"))
height = data.next()
width = data.next()
print height, width

app = QApplication(sys.argv)
scene = QtGui.QGraphicsScene()
rectSize=10
scene.setSceneRect(0,0,rectSize*width,rectSize*height)

rects = []

for i in range(0,height):
    rects.append([])
    for j in range(0,width):
        index = data.next()
        rect = QtGui.QGraphicsRectItem(j*rectSize,i*rectSize,rectSize,rectSize,scene=scene)
        color = palette[index]
        rect.setBrush(QtGui.QBrush(color))
        rects[i].append(rect)

view = QtGui.QGraphicsView()
view.setScene(scene)
view.show()

# Set up a timer to display new solutions
updateTimer = QTimer()
def updateGraph():
    try:
        #ignore the new height and width
        data.next()
        data.next()

        for i in range(0, height):
            for j in range(0, width):
                index = data.next()
                rect = rects[i][j]
                color = palette[index]
                rect.setBrush(QtGui.QBrush(color))
    except:
        updateTimer.stop()

        # Quick hack EK 6/2/2014
        generator = QtSvg.QSvgGenerator() 
        generator.setFileName("morphism.svg")
        generator.setSize(QtCore.QSize(rectSize*width, rectSize*height))
        #generator.setViewBox(QRect(0, 0, rectSize*width, rectSize*height))
        generator.setTitle("SVG Generator Example Drawing")
        generator.setDescription("An SVG drawing created by the SVG Generator")

        painter = QtGui.QPainter()
        painter.begin(generator)
        scene.render(painter)
        painter.end()

updateTimer.timeout.connect(updateGraph)
updateTimer.start(0)

sys.exit(app.exec_())

# FIXME: weird segfault at the end.  I'm probably somehow mismanaging
# those rects.
