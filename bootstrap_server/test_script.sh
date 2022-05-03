#! /bin/bash

bootstrap_port=8096


bin/bootstrap_server eclipse 1 inst 2 24 10 $bootstrap_port &

bin/client localhost $bootstrap_port 0 2 > 1.report &
bin/client localhost $bootstrap_port 0 3 > 2.report &
bin/client localhost $bootstrap_port 0 2 > 3.report &
bin/client localhost $bootstrap_port 0 2 > 4.report &
bin/client localhost $bootstrap_port 1 2 > 5.report &
bin/client localhost $bootstrap_port 1 2 > 6.report &
bin/client localhost $bootstrap_port 2 2 > 7.report &
bin/client localhost $bootstrap_port 2 2 > 8.report &
bin/client localhost $bootstrap_port 2 2 > 9.report &
bin/client localhost $bootstrap_port 2 2 > 10.report &
