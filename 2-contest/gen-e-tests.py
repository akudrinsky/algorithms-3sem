#!python3
import random
import os
import filecmp

settings = {
    'maxVertice': 5,
    'maxEdges': 8,
    'filepath': 'data/input.txt'
}

def genTest():
    nVertice = random.randint(2, settings['maxVertice'])
    nEdges = random.randint(0, settings['maxEdges'])

    sourceUID = random.randint(1, nVertice)
    drainUID = random.randint(1, nVertice)
    while sourceUID == drainUID:
        drainUID = random.randint(1, nVertice)

    with open(settings['filepath'], 'w') as file:
        file.write(f'{nVertice} {nEdges} {sourceUID} {drainUID}\n')
        for i in range(nEdges):
            fromUID = random.randint(1, nVertice)
            toUID = random.randint(1, nVertice)
            file.write(f'{fromUID} {toUID}\n')
        


if __name__ == "__main__":
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