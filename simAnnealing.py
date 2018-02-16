#testing of the simulated annealing approach to the problem

import copy
import random
import math
import numpy as np
class Node:
    def __init__(self, name):
        self.name = name
        self.edges = set()
    def addEdge(self, toNode, cost):
        self.edges.add((toNode, cost))
    

class GraphPartition:
    def __init__(self, nodes):
        #split the lists in half
        self.state = (nodes[:len(nodes)/2], nodes[len(nodes)/2:])

    def successor(self):

        newState = self 

        #get random node from both sides
        randNode1 = random.choice(self.state[0])
        randNode2 = random.choice(self.state[1])

        #remove randomly selected node, add to other side, and return new state
        newState.state[0].remove(randNode1)
        newState.state[1].append(randNode2)
        newState.state[0].append(randNode1)
        newState.state[1].remove(randNode2)


        return newState

    def cost(self):
        total = 0
        for node in self.state[0]:
            for e in node.edges:
                if e[0] in self.state[1]:
                    total += e[1]
        return total
        
#create max nodes
nodes = []
for i in range(1000):
    nodes.append(Node(i))

#create max edges
for i in range(100):
    fromNode = random.choice(nodes)
    toNode   = random.choice(nodes)
    cost = random.randint(1,10)
    fromNode.addEdge(toNode,cost)
    toNode.addEdge(fromNode,cost)

#create starting graph object
g = GraphPartition(nodes)

T = 200

current_cost = g.cost()

global_min = g
global_min_cost = current_cost

#try to find better solutions
for t in range(1,T):
    s = g.successor()
    new_cost = s.cost()
    if new_cost < current_cost:
        g = s
        current_cost = new_cost
    else:
        delta = new_cost - current_cost
        prob = np.exp((delta*T)/t) 
        rand = random.uniform(0,1)
        if rand < prob:
            g = s
            current_cost = new_cost
    if current_cost < global_min_cost:
        global_min = g
        global_min_cost = current_cost
    print current_cost

print "GLOBAL MIN : " + str(global_min_cost)
print "------------------1----------------------"
print sum([len(node.edges) for node in global_min.state[0]])
print "------------------2----------------------"
print sum([len(node.edges) for node in global_min.state[1]])
