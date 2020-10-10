#!python3
import random
import os
import filecmp

settings = {
    'minVert': 3,
    'maxVert': 10,
    'maxCap': 100,
    'filepath': 'data/input.txt'
}

def genEdge(nVert, file):
    fromV = random.randint(1, nVert)
    toV = fromV
    while toV == fromV:
        toV = random.randint(1, nVert)

    file.write(str(fromV) + ' ' + str(toV) + ' ' + str(random.randint(1, settings['maxCap'])) + '\n')


def genTest():
    nVert = random.randint(settings['minVert'], settings['maxVert'])
    nEdges = random.randint(1, 3 * nVert)
    with open(settings['filepath'], 'w') as file:
        file.write(str(nVert) + ' ' + str(nEdges) + '\n')

        for i in range(nEdges):
            genEdge(nVert, file)

if __name__ == "__main__":
    iter = 0
    while True:
        print('Gen test')
        genTest()

        print(iter)
        ++iter
        
        os.system("./smbd <data/input.txt >data/smbd.txt")
        os.system("./a <data/input.txt >data/my.txt")
        
        if filecmp.cmp('data/smbd.txt', 'data/my.txt') == False:
            print('Bad test found')
            break