#! /bin/bash

cd /home/peer/; 
mkdir basalt_project;
cd basalt_project;
cp -r /home/dev/bin ./; 
zip -r src.zip bin/ > /dev/null; 
ansible-playbook /home/dev/send_src.yaml;
echo $@;
ansible all -a "bash -c 'cd /home/peer/basalt_project/bin/; ./honest_node "$@" ;'";
exit;
