#! /bin/bash

# les arguments du script : 
# Ansible_command --port 3000 --machine 192.0.0.1 45  --machine 192.0.0.12 35  --machine 192.0.0.111 77 --machine_malicious 192.0.0.0 33 Choice 1 --info 11 12 23 34
# En cas de modification de nbr de hosts
#    * create_virtual_network.sh : echo "172.17.0.2" >> hosts;
#    * Dockerfile : ssh-copy-id -i ~/.ssh/id_rsa.pub peer@172.17.0.2 < password;

#remove host ip adresses file
rm -f ip_adresses
rm -f hosts


#hosts 
nbr=$#;
nbrup=$(($nbr+7));
# le 7 pour les shifts
for (( i=0; i<$nbrup; i++ ))
    do
        case $1 in

        --port)
            shift
            PORT=$1;
            shift
            ;;

        --machine)
            # host 
            shift
            echo $1 >> hosts;
            shift
            NODE_NBR=$1 
            docker run -itd  \
            --hostname hosts \
            -v "$(pwd):/home/dev" \
            -e NODE_NBR \
            ansible_host;
            shift
            ;;

        --machine_malicious)
            # malicious host 
            shift
            echo $1 >> hosts;
            shift
            j1=$1;
            shift
            j2=$1;
            shift
            j3=$1;
            echo "$PORT";
            docker run -itd \
            --hostname launcher \
            -e NODE_NBR=$j1 \
            -e ATTACK=$j2 \
            -e ATTACK_ID=$j3 \
            -e PORT=$PORT \
            -v "$(pwd):/home/dev" \
            ansible_host;
            shift
            ;;
        --info)
            
            ;;
        esac
        
    done

sleep 2

# main machine
docker run -it \
--hostname launcher \
-v "$(pwd):/home/dev" \
ansible;

./stop_virtual_network.sh;