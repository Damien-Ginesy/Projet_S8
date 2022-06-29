#! /bin/bash

# Création des machines hosts et main

#hosts 
nbr=10;

for (( i=0; i<$nbr; i++ ))
    do
        # host 
        docker run -itd  \
        --hostname hosts \
        -v "$(cd ..; pwd;):/home/dev" \
        ansible_host;
    done

# main machine
docker run -it \
--hostname launcher \
-v "$(cd ..; pwd;):/home/dev" \
ansible;

./stop_virtual_network.sh

# in dev mode execute :
# cd /home/dev/launcher;