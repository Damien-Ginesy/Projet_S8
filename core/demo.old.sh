#!/bin/bash

cd "$(dirname "${BASH_SOURCE[0]}")"

# On kill tout
killall p2p_test
killall p2p_test_m
#kill $(pgrep -f bs_srv.py)
#kill $(pgrep -f server.js)

# Démarrage des services
echo "Lancement..."

n=40
f=3

h=$(($n * (100-$f)/100))
m=$(($n * ($f)/100))

#param basalt
view_size=10
k=1
delay_cycle=100
delay_reset=2000

boostrap_url=127.0.0.1:8080
http_logger_url=127.0.0.1:3000


#bin port viewSize k delay_cycle delay_reset bootstrap_url http_url
b=4000

for i in $(seq 1 $h)
do
#gnome-terminal -- run
./bin/p2p_test $(($b + $i)) $view_size $k $delay_cycle $delay_reset $boostrap_url $http_logger_url> /dev/null &
done
echo "$h noeuds honnete lancé !"

for i in $(seq 1 $m)
do
#gnome-terminal -- run
./bin/p2p_test_m $(($b + $i + $h)) $view_size $k $delay_cycle $delay_reset $boostrap_url $http_logger_url> /dev/null &
done
echo "$m noeuds malicieux lancé !"


wait
echo "Terminé !"
