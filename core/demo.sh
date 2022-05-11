*#!/bin/bash
./stop.sh


echo "Lancement..."

h=990
m=10

#param basalt
view_size=40
k=1
delay_cycle=100
delay_reset=1000

#bin port viewSize k delay_cycle delay_reset bootstrap_url http_url
b=3000

for i in $(seq 1 $h)
do
#gnome-terminal -- run
./bin/p2p_test $(($b + $i)) $view_size $k $delay_cycle $delay_reset 127.0.0.1:8080 127.0.0.1:8081> /dev/null &
done
echo "$h noeuds honnete lancé !"

for i in $(seq 1 $m)
do
#gnome-terminal -- run
./bin/p2p_test_m $(($b + $i + $h)) $view_size $k $delay_cycle $delay_reset 127.0.0.1:8080 127.0.0.1:8081> /dev/null &
done
echo "$m noeuds malicieux lancé !"


wait
echo "Terminé !"
