#! /bin/bash

#remove host ip adresses file
rm -f ip_adresses

# host 1
docker run -itd \
-v "$(pwd):/home/dev" \
ansible;

# host 2
docker run -itd \
-v "$(pwd):/home/dev" \
ansible;

# main machine
docker run -it \
-v "$(pwd):/home/dev" \
ansible \
bash;