# Dockfile documentation
# https://docs.docker.com/engine/reference/builder/

# Ansible documentation
# https://docs.ansible.com/ansible/latest/installation_guide/intro_installation.html

# It must be the first command (except for ARG)
FROM ubuntu

# environment
ENV TZ=Europe/Paris
ENV DEBIAN_FRONTEND=noninteractive

# creating user
RUN \
USER_NAME=peer; \
USER_PASS=basalt; \
echo "$USER_PASS" > password.txt; \
echo "$USER_PASS" >> password.txt; \
useradd -m "$USER_NAME"; \
passwd peer < password.txt > /dev/null; 2> /dev/null;\
rm password.txt;

# Installing
# --- update & upgrade
RUN apt-get update > /dev/null; apt-get upgrade -y > /dev/null;

# --- basic tools
RUN apt-get install -y build-essential python3 python curl grep net-tools iputils-ping zip > /dev/null;

# --- pip
RUN \
apt-get install -y python3-pip;

# --- ssh-client
RUN apt-get install -y ssh-client sshpass;

# --- ansible
RUN pip3 install ansible paramiko argcomplete;

# --- ansible config
ADD ./ansible.cfg /etc/ansible/ansible.cfg

# --- ansible hosts file
ADD ./hosts /etc/ansible/hosts

USER peer

# CMD is the default command that is going to be executed in the startup of every exection.
# Only the last one is considered. So use it once.
CMD \
cd ~;\
echo "basalt" > password.txt; \
mkdir .ssh; \
ssh-keygen -t rsa -f "/home/peer/.ssh/id_rsa" -q -P "" > /dev/null; \
sshpass -f password.txt ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" -o StrictHostKeyChecking=no peer@172.17.0.2 > /dev/null 2> /dev/null; \
sshpass -f password.txt ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" -o StrictHostKeyChecking=no peer@172.17.0.3 > /dev/null 2> /dev/null; \
mkdir /home/peer/basalt_project; \
cp  /home/dev/project_example.c project_example.c; \
cp  /home/dev/Makefile Makefile; \
zip /home/peer/basalt_project/src.zip project_example.c Makefile > /dev/null; \
ansible-playbook /home/dev/send_src.yaml; \
# test kill process
ansible all -a "bash -c '/home/peer/basalt_project/prog & pgrep prog;pkill -9 prog;pgrep prog; echo ""code retour du dernier pgrep est :""$?;'"; \ 
# pratic kill process after clic
#ansible all -a "pkill -9 prog;pgrep prog; echo ""code retour du dernier pgrep est :""$?;'"; \ 
bash


# echo "yes" > ~/password; \
# echo "basalt" >> ~/password; \
# mkdir ~/.ssh; \
# ssh-keygen -t rsa -f "/home/peer/.ssh/id_rsa" -q -P ""; \
# ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" peer@172.17.0.2 < ~/password; \
# ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" peer@172.17.0.3 < ~/password; \
# bash
