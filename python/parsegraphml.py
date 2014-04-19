#!/usr/bin/env python

from xml.dom import minidom
import numpy as np
import re
import sys
from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import QMainWindow, QApplication
from PyQt4.QtCore import QTimer, QObject

if ( len(sys.argv) < 4 ):
    print "Insufficient arguments."
    print "python", sys.argv[0], "<graphml file> <incidence matrix file> <palette file>"
    sys.exit(2)

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

    # Modify the key for sort order.  I want to ensure that "n2" comes
    # before "n10", and "ab5cd2" comes before "ab5cd10".  My solution
    # is to break the key into alternating string and integer parts,
    # and let that be the key.  Thus "ab5cd10" -> ("ab", 5, "cd", 10)
    key = re.split(r"([0-9]*)", eq)

    # Strip out any empty strings
    key = [elem for elem in key if elem != ""]

    # Convert integer strings to bona fide integers
    key = [(int(elem) if elem.isdigit() else elem) for elem in key]
    
    # Make hashable
    key = tuple(key)
    
    return key

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

# Write the matrix to the file
matrixFile = open(sys.argv[2], "w")
matrixFile.write(str(len(eqToIndexMap)) + "\n")

for i in range(0,len(eqToIndexMap)):
    for j in range(0, len(eqToIndexMap)):
        matrixFile.write(str(int(incidences[i][j])) + " "),
    matrixFile.write("\n")
matrixFile.close()

# Determine which color is associated with each matrix index (to
# visualize the output)
indexToColorMap = {}
for node in graph.getElementsByTagName('node'):
    fill = node.getElementsByTagName('y:Fill')[0]
    id = node.attributes['id'].value
    color = fill.attributes['color'].value
    indexToColorMap[idToIndexMap[id]] = "0x" + color.strip("#")
    
paletteFile = open(sys.argv[3], "w")
for index in sorted(indexToColorMap.keys()):
    paletteFile.write(str(index) + " " + indexToColorMap[index] + "\n")
