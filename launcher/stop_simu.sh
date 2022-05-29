#! /bin/bash

pkill bootstrap_serve;

pkill honest_node;
pkill malicious_node;

su peer -c 'ansible all -a "pkill honest_node"';
su peer -c 'ansible all -a "pkill malicious_node"';

# kill metric server
kill $(lsof -t -i :3000);

rm -rf /home/log;

exit;