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
su peer < /etc/ansibleconf.sh; \
cd ~;\
#Launch the Launcher 
mkdir basalt_project; \
cd basalt_project; \
cp -r /home/dev/bootstrap_server ./; \
cd bootstrap_server/bootstrap/; \
npm install > /dev/null; \
npm run start ; \
bash