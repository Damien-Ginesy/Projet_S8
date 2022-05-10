#! /bin/bash

# les arguments du script : 
# Ansible_command --port 3000 --machine USERNAME 192.0.0.1 45  --machine 192.0.0.12 35  --machine 192.0.0.111 77 --attack "type d'attaque" "attack ID" "NBr des noeuds" "Mask si inst"  --info 11 12 23 34
# Ansible_command --port 3000 --ip 172.17.0.12 --machine host_1  192.0.0.1 22  --machine host_2 192.0.0.1 44 --attack eclipse 1 33  --attack inst 1 44 8 --info --taille_vue 11 --nb_cycle_reset 32 --nb_reset 32 --nb_cycle_sec 11 --log_server_hostname "machain" --log server port 8084 (if not 80) 
# En cas de modification de nbr de hosts
#    * create_virtual_network.sh : echo "172.17.0.2" >> hosts;
#    * Dockerfile : ssh-copy-id -i ~/.ssh/id_rsa.pub peer@172.17.0.2 < password;

#remove host ip adresses file
rm -f /etc/ansible/hosts


#hosts 
nbr=$#;
# le 8 pour les shifts
nbrup=$(($nbr+18));
NODE_NBR=0;
#parametre list
bootstrap_param=();
basalt_param=();
j=0;
k=0;

for (( i=0; i<$nbrup; i++ ))
    do
        case $1 in

        --port)
            shift
            BS_PORT=$1;
            shift
            ;;
        --ip)
            shift
            BS_IP=$1;
            shift
            ;;

        --machine)
            # host 
            shift
            USERNAME=$1;
            echo $USERNAME;
            shift
            IP=$1;
            echo $IP;
            echo $1 >> /etc/ansible/hosts;
            shift
            NODE_NBR=$(($NODE_NBR + $1)); 
            shift
            ;;

        --attack)
            # malicious host 
            shift
            TYPE_ATTACK=$1;
            echo $TYPE_ATTACK;
            bootstrap_param[$j]=$TYPE_ATTACK;
            ((j++));
            shift
            ATTACK_ID=$1;
            bootstrap_param[$j]=$ATTACK_ID;
            ((j++));
            shift
            NODE_NBR=$(($NODE_NBR + $1));
            ((j++));
            shift
            String2="inst"
            if [[ "$TYPE_ATTACK" == "$String2" ]]; then
                MASK=$1;
                echo $MASK;
                bootstrap_param[$j]=$MASK;
                ((j++));
                shift
            fi
            ;;
        --info)
            shift
            shift
            VIEW_SIZE=$1;
            basalt_param[$k]=$VIEW_SIZE;
            ((k++));
            shift
            shift
            CYCLE_NBR=$1;
            basalt_param[$k]=$CYCLE_NBR;
            ((k++));
            shift
            shift
            RESET_NBR=$1;
            basalt_param[$k]=$RESET_NBR;
            ((k++));
            shift
            shift
            CYCLE_NBR_SEC=$1;
            basalt_param[$k]=$CYCLE_NBR_SEC;
            ((k++));
            shift
            shift
            log_server_hostname="test";
            basalt_param[$k]=$log_server_hostname;
            ((k++));
            shift
            shift
            log_server_port=8787;
            basalt_param[$k]=$log_server_port;
            ((k++));
            shift

            ;;
        esac
        
    done
cat /etc/ansible/hosts;
echo $NODE_NBR;
bootstrap_param[$j]=$NODE_NBR;
((j++));
bootstrap_param[$j]=$BS_PORT;
((j++));
cd ~/basalt_project/bootstrap_server/;
echo "${bootstrap_param[@]}";
make clean;
make;
stdbuf -o0 ./bin/bootstrap_server "${bootstrap_param[@]}" > output.txt &
#test d'un noeud
sleep 2;

# bin/client launcher $PORT 0 2 > 1.report &
# bin/client launcher $PORT 0 3 > 2.report &
# bin/client launcher $PORT 0 2 > 3.report &
# bin/client launcher $PORT 0 2 > 4.report &
# bin/client launcher $PORT 1 2 > 5.report &
# bin/client launcher $PORT 1 2 > 6.report &
# bin/client launcher $PORT 2 2 > 7.report &
# bin/client launcher $PORT 2 2 > 8.report &
# bin/client launcher $PORT 2 2 > 9.report &
# bin/client launcher $PORT 2 2 > 10.report &
# bin/client launcher $PORT 0 2 > 11.report &
# bin/client launcher $PORT 0 3 > 12.report &
# bin/client launcher $PORT 0 2 > 13.report &
# bin/client launcher $PORT 0 2 > 14.report &
# bin/client launcher $PORT 1 2 > 15.report &
# bin/client launcher $PORT 1 2 > 16.report &
# bin/client launcher $PORT 2 2 > 17.report &
# bin/client launcher $PORT 2 2 > 18.report &
# bin/client launcher $PORT 2 2 > 19.report &


echo "${basalt_param[@]}";



# test BASALT ALGO
#su peer basalt_launcher.sh "${basalt_param[@]}" > output_basalt.txt;
su peer;
# cd /home/peer/; 
# mkdir basalt_project;
# cd basalt_project;
# cp -r /home/dev/bootstrap_server ./; 
# zip -r src.zip bootstrap_server/; 
# ansible-playbook /home/dev/send_src.yaml; 
# ansible all -a "bash -c 'cd /home/peer/basalt_project/bootstrap_server; stdbuf -o0 ./bootstrap_server eclipse 1 inst 2 8 1000 8081 > output.txt ;'" & 
# sleep 2; 
# ansible all -a "bash -c 'cd /home/peer/basalt_project/bootstrap_server; ./node hosts 8081;'"; 
# ansible all -a "bash -c 'cd /home/peer/basalt_project/bootstrap_server; cat output.txt;'"; 
#pratic kill process after clic
# ansible-playbook /home/dev/kill_process.yaml; 
# ansible all -a "echo ""code retour du dernier pgrep est :""$?;'"; 
