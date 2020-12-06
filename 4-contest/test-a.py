import random
import string
import os
import sys

letters = string.ascii_lowercase

def getRandomString(length):
    result_str = ''.join(random.choice(letters) for i in range(length))
    return result_str

if __name__ == "__main__":
    while True:
        if len(sys.argv) == 1:
            print("Input size")
            break
        a = getRandomString(int(sys.argv[1]))
        with open("data/input.txt", "w") as inputFile:
            inputFile.write(a)
        if os.system("./a.out <data/input.txt >data/output.txt") != 0:
            print("ERROR")
            break
