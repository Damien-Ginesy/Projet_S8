# Dockfile documentation
# https://docs.docker.com/engine/reference/builder/

# Ansible documentation
# https://docs.ansible.com/ansible/latest/installation_guide/intro_installation.html

# It must be the first command (except for ARG)
FROM ubuntu:20.04

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
RUN apt-get install -y build-essential cmake python3 curl grep net-tools iputils-ping zip git lsof nmap gnupg wget systemctl > /dev/null;

# --- pip
RUN \
apt-get install -y python3-pip;

# --- ssh server
RUN apt-get install -y openssh-server > /dev/null;

# --- Libstdc++6
RUN \
apt-get install -y libstdc++6;

# --- ssh-client
RUN apt-get install -y ssh-client sshpass;

# --- ansible
RUN pip3 install ansible paramiko argcomplete;

RUN  apt-get -y install sudo;

# --- mongodb
RUN apt-get install -y mongodb > /dev/null;

# launched at a service at the start up of launcher server

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

# --- ssh conf script for ansible
ADD ./ansibleconf.sh /etc/ansibleconf.sh


# Getting code from GitHub & Compiling
RUN \
    echo "Compling / Downloading dependancies"; \
    cd /home/; \
    git clone --branch integration https://github.com/Damien-Ginesy/Projet_S8.git; \
    cd /home/Projet_S8/bootstrap_server/; make > /dev/null; \
    rm -rf /home/Projet_S8/build; \
    cd /home/Projet_S8/; cmake -B build/ > /dev/null; cmake --build build/ > /dev/null; \
    cp /home/Projet_S8/bootstrap_server/bin/bootstrap_server /home/Projet_S8/bin/bootstrap_server; \
    cd /home/Projet_S8/launcher/; npm install > /dev/null; \
    cd /home/Projet_S8/serveur/; npm install > /dev/null;

# --- bins at:
    # bootstrap           :  /home/Projet_S8/bootstrap_server/bin/bootstrap_server, /home/Projet_S8/bin/bootstrap_server
    # basalt honest       :  /home/Projet_S8/bin/honest_node
    # basalt malicious    :  /home/Projet_S8/bin/malicious_node
    # launcher            : cd /home/Projet_S8/launcher npm run start
    # metric              : cd /home/Projet_S8/serveur npm start


# Configuring main maching as a host also

# --- add in host launcher
ADD ./find_free_port.sh /home/peer/find_free_port.sh

# -- add find free port script
ADD ./in_host_launcher.sh /home/peer/in_host_launcher.sh


USER root

# CMD is the default command that is going to be executed in the startup of every exection.
# Only the last one is considered. So use it once.
CMD \

# starting ssh server
/etc/init.d/ssh start; \

# for logs. created at launch
# mkdir /home/log/; \

# configure ssh auth
su peer /etc/ansibleconf.sh; \

#Launch the Launcher
# cd /home/Projet_S8/launcher/; \
# while test 1; do node launcher.js; done & \

# terminal [Optional]
bash