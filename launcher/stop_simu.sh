#! /bin/bash

pkill bootstrap_serve;

su peer -c 'ansible all -a "pkill honest_node"';

su peer -c 'ansible all -a "pkill malicious_node"';

# kill metric server
kill $(lsof -t -i :3000);

rm -rf /home/log;