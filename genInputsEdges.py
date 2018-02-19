file = open("lotsOfEdges.txt","w")
file.write("4 1000\n")

for i in range(500):
    file.write("1 2 1\n")
for i in range(500):
    file.write("3 4 1\n")
