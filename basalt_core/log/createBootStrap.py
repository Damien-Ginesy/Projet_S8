#!/bin/python3
from sys import argv

    
def gen(N: int) -> list:
    return [ ("127.0.0.1", 3000+i, i) for i in range(N) ]
def toFile(filename: str, IDs: list):
    with open(filename, "w") as fOut:
        for i in IDs:
            fOut.write("%s %d %d\n" % i)

def main():
    I = gen(int(argv[1]))
    toFile(argv[2], I)

if __name__ == '__main__':
    main()