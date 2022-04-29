#! /bin/bash

cd /home/peer/; 
mkdir basalt_project;
cd basalt_project;
cp -r /home/dev/bootstrap_server ./; 
zip -r src.zip bootstrap_server/ > /dev/null; 
ansible-playbook /home/dev/send_src.yaml;
exit;