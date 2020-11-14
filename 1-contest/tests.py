#!/Library/Frameworks/Python.framework/Versions/3.8/bin/python3
import random
import os
import filecmp

settings = {
    "minNum": 2,
    "maxNum": 150
}

def getTest():
    with open("dominoes.in", "w") as file:
        file.write("1\n")
        m = random.randint(settings["minNum"], settings["maxNum"])
        n = random.randint(settings["minNum"], settings["maxNum"])
        file.write(f"{m} {n}\n")
        fullNeighbours = [i for i in range(n)]
        for i in range(m):
            for j in range(1, n + 1):
                if random.randint(0, 100) % 2 == True:
                    file.write(f"{j} ")
            file.write("0\n")

while True:
    getTest()
    if os.system("./my < dominoes.in > a.out") != 0:
        print("Test was found")
        break;
    """
    os.system("./lesha < dominoes.in > lesha.out")
    my = open("my.out")
    his = open("lesha.out")
    mySum = 0
    leshaSum = 0
    for i in my.readline().split(' '):
        mySum += int(i)
    for i in his.readline().split(' '):
        leshaSum += int(i)

    if mySum != leshaSum:
        print(f"my {mySum} vs his {leshaSum}")
        break;
    """
