import random

n = input("How many vertecies > ")
m = input("How many edges > ")

print str(n) + " " + str(m)

for i in range(m):
    fromNode = random.choice(range(1,n+1))
    toNode = random.choice(range(1,n+1))
    while toNode == fromNode:
        toNode = random.choice(range(1,n+1))
    cost = random.randint(1,50)
    print str(fromNode) + " " + str(toNode) + " " + str(cost)
