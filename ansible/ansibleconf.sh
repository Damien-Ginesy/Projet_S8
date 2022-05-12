#! /bin/bash

cd /home/peer/; 
echo "basalt" > password.txt; 
mkdir .ssh; 
ssh-keygen -t rsa -f "/home/peer/.ssh/id_rsa" -q -P "" > /dev/null; 
sshpass -f password.txt ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" -o StrictHostKeyChecking=no peer@172.17.0.2 > /dev/null 2> /dev/null; 
sshpass -f password.txt ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" -o StrictHostKeyChecking=no peer@172.17.0.3 > /dev/null 2> /dev/null;
sshpass -f password.txt ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" -o StrictHostKeyChecking=no peer@172.17.0.4 > /dev/null 2> /dev/null;
sshpass -f password.txt ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" -o StrictHostKeyChecking=no peer@172.17.0.5 > /dev/null 2> /dev/null;
sshpass -f password.txt ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" -o StrictHostKeyChecking=no peer@172.17.0.6 > /dev/null 2> /dev/null;
sshpass -f password.txt ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" -o StrictHostKeyChecking=no peer@172.17.0.7 > /dev/null 2> /dev/null;
sshpass -f password.txt ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" -o StrictHostKeyChecking=no peer@172.17.0.8 > /dev/null 2> /dev/null;
sshpass -f password.txt ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" -o StrictHostKeyChecking=no peer@172.17.0.9 > /dev/null 2> /dev/null;
sshpass -f password.txt ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" -o StrictHostKeyChecking=no peer@172.17.0.10 > /dev/null 2> /dev/null;
sshpass -f password.txt ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" -o StrictHostKeyChecking=no peer@172.17.0.11 > /dev/null 2> /dev/null; 
exit