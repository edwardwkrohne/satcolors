#!/usr/bin/env python

from xml.dom import minidom
import numpy as np
import sys
from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import QMainWindow, QApplication
from PyQt4.QtCore import QTimer, QObject

if ( len(sys.argv) < 2 ):
    print "No file to process provided.  Exiting."
    sys.exit()

dom = minidom.parse(open(sys.argv[1],'r'))
root = dom.getElementsByTagName("graphml")[0]
graph = root.getElementsByTagName("graph")[0]

eqKeyId=None
eqDefault=None
for key in root.getElementsByTagName("key"):
    if not key.hasAttribute('attr.name') or key.attributes['attr.name'].value != 'Equivalence':
        continue

    if not key.hasAttribute('for') or key.attributes['for'].value != 'node':
        continue

    if not key.hasAttribute('id'):
        continue

    eqKeyId = key.attributes['id'].value
    eqDefault = key.getElementsByTagName('default')[0].childNodes[0].nodeValue
    break

def getEqKey(node):
    # Find the "equivalence" data, or, if not found, go with the default equivalence.
    eq = None
    for datum in node.getElementsByTagName('data'):
        if datum.attributes['key'].value == eqKeyId:
            basicEq = datum.childNodes[0].nodeValue            
            eq = datum.childNodes[0].nodeValue
            break
    else:
        eq = eqDefault

    # Now find if this is a real equivalence, or one of the catch-all
    # equivalences.
    if ( int(eq) < 0 ):
        eq = node.attributes['id'].value

    return eq    

# Build a table of all the nodes and their equivalence classes
idToEqMap = {}
for node in graph.getElementsByTagName('node'):
    idToEqMap[node.attributes['id'].value] = getEqKey(node)

# Build a table of all equivalence classes and an index suitable for
# using as a matrix row/column number
eqToIndexMap = {}
i = 0
for eq in sorted(set(idToEqMap.values())):
    eqToIndexMap[eq] = i
    i+=1

# Build a table mapping node ids to their equivalence index
idToIndexMap = {}
for id in idToEqMap.keys():
    idToIndexMap[id] = eqToIndexMap[idToEqMap[id]]

# Run through the pseudonodes, build the matrix.
incidences = np.zeros((len(eqToIndexMap),len(eqToIndexMap)))

for edge in graph.getElementsByTagName('edge'): 
    sourceId=edge.attributes['source'].value
    sourceIndex=idToIndexMap[sourceId]

    targetId=edge.attributes['target'].value
    targetIndex=idToIndexMap[targetId]

    incidences[sourceIndex][targetIndex] = 1
    incidences[targetIndex][sourceIndex] = 1

print len(eqToIndexMap)

for i in range(0,len(eqToIndexMap)):
    for j in range(0, len(eqToIndexMap)):
        print int(incidences[i][j]),
    print ""

# Determine which color is associated with each matrix index (to
# visualize the output)
indexToColorMap = {}
for node in graph.getElementsByTagName('node'):
    fill = node.getElementsByTagName('y:Fill')[0]
    id = node.attributes['id'].value
    indexToColorMap[idToIndexMap[id]] = fill.attributes['color'].value
    
for index in sorted(indexToColorMap.keys()):
    print index, indexToColorMap[index]

##########################################################################
##
##  FIXME: Put code to actually find the solution here!!
##
##########################################################################

# For the moment (FIXME this is totally ridiculous, I know), just grab
# the precooked output from output.dat.

# Interpret the solution

def outputData():
    f = open("outputdata", "r")
    for line in f:
        for datum in line.strip().split():
            yield int(datum)

data = outputData()
height = data.next()
width = data.next()

# This is the output the solver came up with
#output = []

print height, width

#for i in range(0,height):
#    output.append([])
#    for j in range(0,width):
#        output[i].append(data.next())

#print output

app = QApplication(sys.argv)

scene = QtGui.QGraphicsScene()
scene.setSceneRect(0,0,20*height,20*width)

for i in range(0,height):
    #output.append([])
    for j in range(0,width):
        index = data.next()
        rect = QtGui.QGraphicsRectItem(j*20,i*20,20,20,scene=scene)
        colorStr = indexToColorMap[index]
        # FIXME check to make sure this is a valid color string!
        color = QtGui.QColor.fromRgb(int(colorStr[1:], 16))
        rect.setBrush(QtGui.QBrush(color))

view = QtGui.QGraphicsView()
view.setScene(scene)
view.show()
sys.exit(app.exec_())
