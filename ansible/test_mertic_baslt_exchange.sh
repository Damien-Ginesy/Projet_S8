#! /bin/bash

bootstrap_port=8097;


/home/components/bootstrap_server/bin/bootstrap_server inst 2 24 4 $bootstrap_port > /home/log/bootstrap &

# /home/components/bin/honest_node 3 $(/home/components/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port localhost 3000 > /dev/null &
# /home/components/bin/honest_node 3 $(/home/components/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port localhost 3000 > /dev/null &
# /home/components/bin/honest_node 3 $(/home/components/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port localhost 3000 > /dev/null &
# /home/components/bin/honest_node 3 $(/home/components/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port localhost 3000 > /dev/null &

# /home/components/bin/malicious_node 3 $(/home/components/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port localhost 3000 1 > /dev/null &
# /home/components/bin/malicious_node 3 $(/home/components/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port localhost 3000 1 > /dev/null &

# /home/components/bin/malicious_node 3 $(/home/components/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port localhost 3000 2 &
# /home/components/bin/malicious_node 3 $(/home/components/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port localhost 3000 2 > /dev/null &
# /home/components/bin/malicious_node 3 $(/home/components/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port localhost 3000 2 > /dev/null &
# /home/components/bin/malicious_node 3 $(/home/components/ansible/find_free_port.sh) 10 1 1 localhost $bootstrap_port localhost 3000 2 > /dev/null &
