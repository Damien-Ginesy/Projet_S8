#! /bin/bash

bootstrap_port=8097;


/home/Projet_S8/bootstrap_server/bin/bootstrap_server eclipse 1 inst 2 24 10 $bootstrap_port > /home/log/bootstrap &

/home/Projet_S8/bin/honest_node 3 $(/home/Projet_S8/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port localhost 3000 > /dev/null &
/home/Projet_S8/bin/honest_node 3 $(/home/Projet_S8/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port localhost 3000 > /dev/null &
/home/Projet_S8/bin/honest_node 3 $(/home/Projet_S8/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port localhost 3000 > /dev/null &
/home/Projet_S8/bin/honest_node 3 $(/home/Projet_S8/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port localhost 3000 > /dev/null &

/home/Projet_S8/bin/malicious_node 3 $(/home/Projet_S8/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port 1 localhost 3000 1 > /dev/null &
/home/Projet_S8/bin/malicious_node 3 $(/home/Projet_S8/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port 1 localhost 3000 1 > /dev/null &

/home/Projet_S8/bin/malicious_node 3 $(/home/Projet_S8/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port 2 localhost 3000 > /dev/null &
/home/Projet_S8/bin/malicious_node 3 $(/home/Projet_S8/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port 2 localhost 3000 > /dev/null &
/home/Projet_S8/bin/malicious_node 3 $(/home/Projet_S8/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port 2 localhost 3000 > /dev/null &
/home/Projet_S8/bin/malicious_node 3 $(/home/Projet_S8/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port 2 localhost 3000 > /dev/null &
