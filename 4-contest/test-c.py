import random
import os
import sys
import filecmp
from itertools import combinations

def getRandomArray(length, numbers):
    result_str = [random.choice(numbers) for i in range(length)]
    return result_str

def genRandomTests():
    nCharacters = 10
    numbers = [i for i in range(1, nCharacters + 1)]

    while True:
        nLetters = random.randint(1000, 1500)
        a = getRandomArray(nLetters, numbers)
        with open("data/input.txt", "w") as inputFile:
            inputFile.write(str(nLetters) + ' '+ str(nCharacters) + '\n')
            inputFile.write(' '.join(str(x) for x in a))

        if os.system("./my <data/input.txt >data/output.txt") != 0:
            print("ERROR")
            break

        continue
        os.system("./smbd <data/input.txt >data/smbd.txt")
        os.system("./my <data/input.txt >data/my.txt")
        
        my = open('data/my.txt', 'r')
        smbd = open('data/smbd.txt', 'r')
        
        if (my.readline() != smbd.readline()):
            break



if __name__ == "__main__":
    genRandomTests()