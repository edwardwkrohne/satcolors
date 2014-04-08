#!/usr/bin/env python

from xml.dom import minidom
import numpy as np

dom = minidom.parse(open('Gamma3.graphml','r'))
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
