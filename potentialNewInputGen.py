import random

file = open("potentialNewMaxInputs.txt","w")
file.write("1000 100000\n")
inputList = []
for i in range (1,1001):
    for j in range (i+1,1001):
        cost = random.randint(1,50)
        inputList.append(str(i) + " " + str(j) + " " + str(cost)+ "\n")
while(len(inputList) > 100000):
    print(len(inputList))
    toRemove = random.randint(1,len(inputList)-1)
    del inputList[toRemove]
for i in inputList:
    file.write(i)
