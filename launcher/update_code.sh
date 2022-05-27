#! /bin/bash

cd /home/Projet_S8;
git pull origin integration;

cd /home/Projet_S8/bootstrap_server/; make > /dev/null;

cd /home/Projet_S8/; cmake -B build/ > /dev/null; cmake --build build/ > /dev/null;

cd /home/Projet_S8/launcher/; npm install > /dev/null;
cd /home/Projet_S8/serveur/; npm install > /dev/null;


# close every thing but the launcher

exit;