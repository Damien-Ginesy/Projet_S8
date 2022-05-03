#! /bin/bash

# les arguments du script : 
# Ansible_command --port 3000 --machine 192.0.0.1 45  --machine 192.0.0.12 35  --machine 192.0.0.111 77 --attack "type d'attaque" "attack ID" "NBr des noeuds" "Mask si inst"  --info 11 12 23 34
# En cas de modification de nbr de hosts
#    * create_virtual_network.sh : echo "172.17.0.2" >> hosts;
#    * Dockerfile : ssh-copy-id -i ~/.ssh/id_rsa.pub peer@172.17.0.2 < password;

#remove host ip adresses file
rm -f ip_adresses
rm -f hosts


#hosts 
nbr=10;

for (( i=0; i<$nbr; i++ ))
    do
        # host 
        docker run -itd  \
        --hostname hosts \
        -v "$(pwd):/home/dev" \
        ansible_host;
    done

# main machine
docker run -it \
--hostname launcher \
-v "$(pwd):/home/dev" \
ansible;

./stop_virtual_network.sh