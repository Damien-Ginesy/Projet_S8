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
n = 9
m = 1
httpPort = 8081

def honest_launcher(port):
    cmd = "./bin/honest_node %d %d %d %d %d %s %d localhost %d > /dev/null &" % (viewSize, port, 5*freq, k, freq, bsHost, bsPort, httpPort)
    system(cmd)

def malicious_launcher(port):
    cmd = "./bin/malicious_node %d %d %d %d %d %s %d 1 localhost %d > /dev/null &" % (viewSize, port, 5*freq, k, freq, bsHost, bsPort, httpPort)
    system(cmd)

def bsLauncher(port):
    if m:
        cmd = "./bootstrap_server/bin/bootstrap_server eclipse 1 %d %d" % (n+m, port)
    else:
        cmd = "./bootstrap_server/bin/bootstrap_server %d %d" % (n+m, port)
    system(cmd)

bsServverThread = Thread(target=bsLauncher, args=(bsPort,))

bsServverThread.start()

for i in range(n):
    honest_launcher(p)
    p+=1

for i in range(m):
    malicious_launcher(p)
    p+=1

bsServverThread.join()

while 1:
    try:
        sleep(1)
    except KeyboardInterrupt:
        system("killall honest_node && killall malicious_node")
        break