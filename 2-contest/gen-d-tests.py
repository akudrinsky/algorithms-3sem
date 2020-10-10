#!python3
import random
import os
import filecmp

settings = {
    'maxRowNum': 10,
    'maxColNum': 10,
    'filepath': 'data/input.txt'
}

def genTest():
    nRows = random.randint(1, settings['maxRowNum'])
    nCols = random.randint(1, settings['maxColNum'])

    chars = ['.', 'H', 'C', 'O', 'N']

    with open(settings['filepath'], 'w') as file:
        file.write(str(nRows) + ' ' + str(nCols) + '\n')

        for row in range(nRows):
            for col in range(nCols):
                val = random.choice(chars)
                file.write(val)
            file.write('\n')
        


if __name__ == "__main__":
    iter = 0
    while True:
        genTest()

        print('Generated test' + str(iter))
        ++iter
        
        os.system("./smbd <data/input.txt >data/smbd.txt")
        os.system("./my <data/input.txt >data/my.txt")
        
        if filecmp.cmp('data/smbd.txt', 'data/my.txt') == False:
            print('Bad test found')
            break