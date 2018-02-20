import random

groups = 28

for g in range(1,groups+1):

    fileName = "inputs/input_group" + str(g) + ".txt"
    file = open(fileName, "w")

    n = random.randint(1, 500) * 2
    m = random.randint(0,100000)

    file.write(str(n) + " " + str(m) + "\n")

    for e in range(m):
        fromNode = random.randint(1,n)
        toNode = random.randint(1,n)
        while fromNode == toNode:
            toNode = random.randint(1,n)
        w = random.randint(1,50)
        file.write(str(fromNode) + " " + str(toNode) + " " + str(w) + "\n")

