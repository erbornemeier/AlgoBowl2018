import random

n = input("How many vertecies > ")
m = input("How many edges > ")

file = open("inputs.txt","w")
file.write(str(n) + " " + str(m) + "\n")

for i in range(m):
    fromNode = random.choice(range(1,n+1))
    toNode = random.choice(range(1,n+1))
    while toNode == fromNode:
        toNode = random.choice(range(1,n+1))
    cost = random.randint(1,50)
    file.write(str(fromNode) + " " + str(toNode) + " " + str(cost) + "\n")
