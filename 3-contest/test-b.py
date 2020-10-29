#!python3
import random
import os
import filecmp



settings = {
    'nColors': 10,
    'nElems': 10,
    'filepath': 'data/input.txt'
}

def genTest():
    nColors = random.randint(1, settings['nColors'])
    nElems = random.randint(1, settings['nElems'])
    with open(settings['filepath'], 'w') as file:
        file.write(f'{nElems} {nColors}\n')

        for i in range(nElems):
            file.write(f'{nColors} ')
            #a = random.randint(1, nColors + 1)
            #file.write(f'{a} ')

def solveTest():
    with open(settings['filepath'], 'r') as file:
        outputFile = open('data/smbd.txt', 'w')

        a = [int(i) for i in file.readline().split(' ') if i != '']
        nColors = a[1]
        nElems = a[0]
        
        a = [int(i) for i in file.readline().split(' ') if i != '']
        for i in range(int(nElems / 2) - 1, -1, -1):
            first = a[i :: -1]
            second = a[i + 1 : 2 * i + 2 : 1]
            #print(first, second)
            if first == second:
                outputFile.write(f'{nElems - i - 1} ')

        outputFile.write(f'{nElems} ')
        outputFile.close()

            

if __name__ == "__main__":
    iter = 0
    while True:
        #print('Gen test')
        genTest()
        solveTest()

        print(iter)
        iter += 1

        os.system("./a.out <data/input.txt >data/my.txt")
        
        if filecmp.cmp('data/smbd.txt', 'data/my.txt') == False:
            print('Bad test found')
            break