#! /bin/bash

cd /home/peer/; 
echo "basalt" > password.txt; 
mkdir .ssh; 
ssh-keygen -t rsa -f "/home/peer/.ssh/id_rsa" -q -P "" > /dev/null; 
sshpass -f password.txt ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" -o StrictHostKeyChecking=no peer@172.17.0.2 > /dev/null 2> /dev/null; 
sshpass -f password.txt ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" -o StrictHostKeyChecking=no peer@172.17.0.3 > /dev/null 2> /dev/null; 
exit