#! /bin/bash

bootstrap_port=8096


./bootstrap_server eclipse 1 inst 2 24 10 $bootstrap_port &

./client localhost $bootstrap_port 0 2 > 1.report &
./client localhost $bootstrap_port 0 3 > 2.report &
./client localhost $bootstrap_port 0 2 > 3.report &
./client localhost $bootstrap_port 0 2 > 4.report &
./client localhost $bootstrap_port 1 2 > 5.report &
./client localhost $bootstrap_port 1 2 > 6.report &
./client localhost $bootstrap_port 2 2 > 7.report &
./client localhost $bootstrap_port 2 2 > 8.report &
./client localhost $bootstrap_port 2 2 > 9.report &
./client localhost $bootstrap_port 2 2 > 10.report &
