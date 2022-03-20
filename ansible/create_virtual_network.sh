#! /bin/bash

# En cas de modification de nbr de hosts
#    * create_virtual_network.sh : echo "172.17.0.2" >> hosts;
#    * Dockerfile : ssh-copy-id -i ~/.ssh/id_rsa.pub peer@172.17.0.2 < password;

#remove host ip adresses file
rm -f ip_adresses
rm -f hosts

# host 1
docker run -itd \
-v "$(pwd):/home/dev" \
ansible_host;
echo "172.17.0.2" >> hosts;

# host 2
docker run -itd \
-v "$(pwd):/home/dev" \
ansible_host;
echo "172.17.0.3" >> hosts;

sleep 2

# main machine
docker run -it \
--hostname launcher \
-v "$(pwd):/home/dev" \
ansible;

./stop_virtual_network.sh;