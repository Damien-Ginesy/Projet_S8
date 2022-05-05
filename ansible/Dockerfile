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
useradd -m "$USER_NAME" && echo "peer:peer" | chpasswd && adduser peer sudo;\
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

RUN  apt-get -y install sudo;

# --- nodejs and npm
ENV NODE_VERSION=16.13.0
RUN apt install -y curl > /dev/null;
RUN curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.0/install.sh | bash > /dev/null;
ENV NVM_DIR=/root/.nvm
RUN . "$NVM_DIR/nvm.sh" && nvm install ${NODE_VERSION} > /dev/null;
RUN . "$NVM_DIR/nvm.sh" && nvm use v${NODE_VERSION} > /dev/null;
RUN . "$NVM_DIR/nvm.sh" && nvm alias default v${NODE_VERSION} > /dev/null;
ENV PATH="/root/.nvm/versions/node/v${NODE_VERSION}/bin/:${PATH}"
RUN node --version
RUN npm --version

# --- ansible config
ADD ./ansible.cfg /etc/ansible/ansible.cfg

# --- ansible conf file
ADD ./ansibleconf.sh /etc/ansibleconf.sh


USER root

# CMD is the default command that is going to be executed in the startup of every exection.
# Only the last one is considered. So use it once.
CMD \
#whoami;\
su peer < /etc/ansibleconf.sh; \
#su peer;\
#whoami;\
#Ansible configuration
# cd ~;\
# pwd ;\
# ls; \
# echo "basalt" > password.txt; \
# ls;\
# mkdir .ssh; \
# cd .ssh;\
# ssh-keygen -t rsa -f "/home/peer/.ssh/id_rsa" -q -P "" > /dev/null; \
# sshpass -f password.txt ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" -o StrictHostKeyChecking=no peer@172.17.0.2 > /dev/null 2> /dev/null; \
# sshpass -f password.txt ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" -o StrictHostKeyChecking=no peer@172.17.0.3 > /dev/null 2> /dev/null; \
cd ~;\
#Launch the Launcher 
mkdir basalt_project; \
cd basalt_project; \
cp -r /home/dev/bootstrap_server ./; \
cd bootstrap_server/bootstrap/; \
#npm install > /dev/null; \
#npm run start ; \
# TEST LAUNCHER DIRECT
# cd ~/basalt_project/bootstrap_server/; \
# chmod ugo+rwx launcher.sh;\
# ./launcher.sh --port 8082 --machine peer 172.17.0.2 33 --machine peer 172.17.0.3 33 --machine peer 172.17.0.4 33 --attack eclipse 1 50 --attack inst 2 20 8 --info --taille_vue 11 --nb_cycle_reset 12 --nb_reset 23 --nb_cycle_sec 34; \
#cp  /home/dev/project_example.c project_example.c; \
#cp  /home/dev/Makefile Makefile; \
#zip /home/peer/basalt_project/src.zip project_example.c Makefile > /dev/null; \
# test BASALT ALGO
# zip -r /home/peer/basalt_project/src.zip bootstrap_server/; \
# ansible-playbook /home/dev/send_src.yaml; \
# ansible all -a "bash -c 'cd /home/peer/basalt_project/bootstrap_server; stdbuf -o0 ./bootstrap_server eclipse 1 inst 2 8 1000 > output.txt ;'" & \
# sleep 2; \
# ansible all -a "bash -c 'cd /home/peer/basalt_project/bootstrap_server; ./node hosts 8081;'"; \ 
# ansible all -a "bash -c 'cd /home/peer/basalt_project/bootstrap_server; cat output.txt;'"; \ 
# # pratic kill process after clic
# #ansible-playbook /home/dev/kill_process.yaml; \
# #ansible all -a "echo ""code retour du dernier pgrep est :""$?;'"; \ 
# echo "yes" > ~/password; \
# echo "basalt" >> ~/password; \
# mkdir ~/.ssh; \
# ssh-keygen -t rsa -f "/home/peer/.ssh/id_rsa" -q -P ""; \
# ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" peer@172.17.0.2 < ~/password; \
# ssh-copy-id -i "/home/peer/.ssh/id_rsa.pub" peer@172.17.0.3 < ~/password; \
bash