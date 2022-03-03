# Dockfile documentation
# https://docs.docker.com/engine/reference/builder/

# Ansible documentation
# https://docs.ansible.com/ansible/latest/installation_guide/intro_installation.html

# It must be the first command (except for ARG)
FROM ubuntu

# Installing
# --- update & upgrade
RUN apt-get update > /dev/null; apt-get upgrade -y > /dev/null;

# --- basic tools
RUN apt-get install -y build-essential python3 python curl net-tools > /dev/null;

# --- pip
RUN \
apt-get install -y python3-pip;

# --- ansible
RUN pip3 install ansible paramiko argcomplete;

# --- ansible conf file (ansible.cfg)
ADD ansible.cfg /etc/ansible/ansible.cfg

# CMD is the default command that is going to be executed in the startup of every exection.
# Only the last one is considered. So use it once.
CMD \
bash
