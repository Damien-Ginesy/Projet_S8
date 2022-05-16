#!/bin/python3
from os import system, getcwd
from time import sleep
from sys import argv
from threading import Thread

bsPort = 5000
bsHost = "localhost"
viewSize = 1
k = 0
freq = 1
p = 3000
n = 2
m = 0
httpPort = 8081

def honest_launcher(port):
    cmd = "./bin/honest_node %d %d %d %d %d %s %d localhost %d &" % (viewSize, port, 5*freq, k, freq, bsHost, bsPort, httpPort)
    system(cmd)

def malicious_launcher(port):
    cmd = "./bin/malicious_node %d %d %d %d %d %s %d 1 localhost %d &" % (viewSize, port, 5*freq, k, freq, bsHost, bsPort, httpPort)
    system(cmd)

for i in range(n):
    honest_launcher(p)
    p+=1

for i in range(m):
    malicious_launcher(p)
    p+=1

while 1:
    try:
        sleep(1)
    except KeyboardInterrupt:
        system("killall honest_node && killall malicious_node")
        break