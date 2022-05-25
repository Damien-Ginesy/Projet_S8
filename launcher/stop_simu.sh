#! /bin/bash

pkill bootstrap_serve;

# ansible all -a "pkill honest_node; pkill malicious_node;"

# kill metric server
kill $(lsof -t -i :3000);