#!/bin/env python
import argparse
import time
import os

class TouchScreen:
    def __init__(self, fifoName):
        self.fifo = open(fifoName, "w")
        # self.read = open("{}.sh".format(fifoName), "r")
        # self.read.seek(0, 2)
        fd = os.open("{}.sh".format(fifoName), os.O_RDWR|os.O_CREAT)
        self.read = os.fdopen(fd, "w+")
        
        print("Pokemat init")
        
    def click(self, x, y, button = 1, duration = 50):
        self.fifo.write("click:{},{},{},{}\n".format(x,y,button, duration))
        self.fifo.flush()
        
    def getRGB(self, x, y):
        # print("color:{},{}\n".format(x,y))
        self.fifo.writelines("color:{},{}\n".format(x,y))
        self.fifo.flush()
        time.sleep(2)
        print(self.read.read())
        
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--fifo", action="store", required=True)
    args = parser.parse_args()
    print(args)
    ts = TouchScreen(args.fifo)
    # ts.click(200,200)
    for y in range(200,204,1):
        print(y)
        ts.getRGB(200,y)
    
if __name__ == "__main__":
    main()
    