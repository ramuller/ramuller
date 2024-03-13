#!/bin/env python
import argparse

class TouchScreen:
    def __init__(self, fifoName):
        self.fifo = open(fifoName, "w")
        
    def click(self, x, y, button = 1, duration = 50):
        self.fifo.write("click:{},{},{},{}".format(x,y,button, duration))

        
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--fifo", action="store", required=True)
    args = parser.parse_args()
    print(args)
    ts = TouchScreen(args.fifo)
    ts.click(200,200)
    
if __name__ == "__main__":
    main()
    