#!/usr/bin/env python

from xml.dom import minidom

dom = minidom.parse(open('Gamma3.graphml','r'))
root = dom.getElementsByTagName("graphml")[0]
graph = root.getElementsByTagName("graph")[0]

eqKeyId=None
defaultEq="0" # The "equivalence" is absent if zero, so I have to keep track of that.

for key in root.getElementsByTagName("key"):
    if not key.hasAttribute('attr.name') or key.attributes['attr.name'].value != 'Equivalence':
        continue

    if not key.hasAttribute('for') or key.attributes['for'].value != 'node':
        continue

    if not key.hasAttribute('id'):
        continue

    eqKeyId = key.attributes['id'].value
    defaultEq = key.getElementsByTagName("default")[0].childNodes[0].nodeValue

    print defaultEq
    break

print eqKeyId

class PseudoNode:
    def __init__(self):
        self.nodes = []
        self.adjNodes = []
        self.color = "#000000"
        self.equiv = None

# Build a table of all the nodes and their equivalence classes
pseudoNodes = {}
for node in graph.getElementsByTagName('node'):
    for datum in node.getElementsByTagName('data'):
        if datum.attributes['key'].value == eqKeyId:
            equivalence = int(datum.childNodes[0].nodeValue)
            if equivalence >= 0:
                if not equivalence in pseudoNodes:
                    pseudoNodes[equivalence] = PseudoNode()
                pseudoNodes[equivalence].nodes.append(node)
            else:
                pn = PseudoNode()
                pn.nodes.append(node)
                pseudoNodes[node.attributes['id'].value] = pn
            continue

# Note: misses node zero because the file is weird; 
# it doesn't redundantly specify unnecessary elements.

for pn in pseudoNodes.keys():
    print (str(pn)+": "+str(pseudoNodes[pn].nodes))

