#!python3
import random
import os
import filecmp
import string

settings = {
    'nLines': 5,
    'maxLen': 10,
    'filepath': 'data/input.txt'
}

letters = list(string.ascii_lowercase)

def genTest():
    nLines = random.randint(1, settings['nLines'])
    maxLen = random.randint(1, settings['maxLen'])

    with open(settings['filepath'], 'w') as file:
        file.write(random.sample(letters, random.randint(1, maxLen))
        file.write(str(nLines))
        for i in range(nLines):
            file.write(random.sample(letters, random.randint(1, maxLen))
        


if __name__ == "__main__":
    #os.system("g++ -std=c++17 3-contest/e.cpp -o my")
    #os.system("g++ -std=c++17 3-contest/smbd.cpp -o smbd")
    iter = 0
    while True:
        genTest()

        print('Generated test ' + str(iter))
        iter += 1
        
        os.system("./smbd <data/input.txt >data/smbd.txt")
        os.system("./my <data/input.txt >data/my.txt")
        
        if filecmp.cmp('data/smbd.txt', 'data/my.txt') == False:
            print('Bad test found')
            break