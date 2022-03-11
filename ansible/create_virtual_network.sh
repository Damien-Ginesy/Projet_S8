#! /bin/bash

#remove host ip adresses file
rm -f ip_adresses
rm -f hosts

# host 1
docker run -itd \
-v "$(pwd):/home/dev" \
ansible;
echo "172.17.0.2" >> hosts

# host 2
docker run -itd \
-v "$(pwd):/home/dev" \
ansible;
echo "172.17.0.3" >> hosts

# main machine
docker run -it \
-v "$(pwd):/home/dev" \
ansible \
bash;

./stop_virtual_network.sh