#! /bin/bash

cd /home/peer/; 
mkdir basalt_project;
cd basalt_project;
cp -r /home/dev/cpp_sim ./; 
zip -r src.zip cpp_sim/ > /dev/null; 
ansible-playbook /home/dev/send_src.yaml;
echo $@;
ansible all -a "bash -c 'cd /home/peer/basalt_project/cpp_sim/bin; ./basalt_ccp_sim $@ ;'";
exit;