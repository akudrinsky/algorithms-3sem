#!python3
import random
import os
import filecmp

settings = {
    'nPeople': 3,
    'nTees': 3,
    'maxTea': 5,
    'filepath': 'data/input.txt'
}

def genTest():
    nPeople = random.randint(1, settings['nPeople'])
    nTees = random.randint(1, settings['nTees'])

    with open(settings['filepath'], 'w') as file:
        file.write(str(nPeople) + ' ' + str(nTees) + '\n')

        for teeID in range(nTees):
            file.write(str(random.randint(1, settings['maxTea'])) + ' ')
        file.write('\n')

        tees = [str(i) for i in range(1, nTees + 1)]

        for person in range(nPeople):
            sampleSize = random.randint(0, nTees)
            subTees = random.sample(tees, sampleSize)
            file.write(str(sampleSize) + ' ')
            for elem in subTees:
                file.write(elem + ' ')
            file.write('\n')

            

if __name__ == "__main__":
    iter = 0
    while True:
        genTest()

        print('Generated test ' + str(iter))
        ++iter
        
        os.system("./smbd <data/input.txt >data/smbd.txt")
        os.system("./my <data/input.txt >data/my.txt")
        
        if filecmp.cmp('data/smbd.txt', 'data/my.txt') == False:
            print('Bad test found')
            break