import FileClassificator as FC
import sys

# print(sys.path)

def progresser(x,y,z):
    print('Current State: %s, Progress %d%%, Info: %s' % (x, int(y*100), z))

moved = FC.Classificate('../wallpaper', '../newDir', progresser)
print(moved)