#!/bin/python3
from os import system, getcwd
from sys import argv
from threading import Thread
import createBootStrap

def launcher(filePath: str, viewSize: int, nodeID: int):
    logFileName = "%s/log/%d.log" % (getcwd(), nodeID)
    cmd = "%s/bin/p2p_test %d %d %s > %s" % (getcwd(), nodeID, viewSize, filePath, logFileName)
    print(cmd)
    system(cmd)

if len(argv) < 4:
    print("Usage:\nlaunch.py <N> <viewSize> <bootstrapFile>")
    exit()
N = int(argv[1])
viewSize = int(argv[2])
IDs = createBootStrap.gen(N)
createBootStrap.toFile(argv[3], IDs)


T = []

for i in IDs:
    T.append(Thread(target=launcher, args=(argv[3], viewSize, i[2])))
for t in T:
    t.start()
for t in T:
    t.join()