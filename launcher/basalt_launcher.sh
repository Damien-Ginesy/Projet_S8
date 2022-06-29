#! /bin/bash

cd /home/peer/; 
mkdir basalt_project;
cd basalt_project;
cp -r /home/dev/bin ./; 
zip -r src.zip bin/ > /dev/null; 
echo $@ > basalt_param.txt;
ansible-playbook /home/dev/send_src.yaml > basalt_output.txt;
ansible all -a "bash -c 'cd /home/peer/basalt_project/bin/; stdbuf -o0 ./honest_node "$@";'";
exit;
