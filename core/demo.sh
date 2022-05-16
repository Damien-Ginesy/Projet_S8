#!/bin/bash
./stop.sh


echo "Lancement..."

h=90
m=10

#param basalt
view_size=4
k=1
delay_cycle=100
delay_reset=1000

#bin port viewSize k delay_cycle delay_reset bootstrap_url http_url
b=3002

for i in $(seq 1 $h)
do
#gnome-terminal -- run
./bin/honest_node $view_size $(($b + $i)) 10 $k 1 127.0.0.1 3001 127.0.0.1 3000 > /dev/null &
done
echo "$h noeuds honnete lancé !"

for i in $(seq 1 $m)
do
#gnome-terminal -- run
./bin/malicious_node $view_size $(($b + $i +$h)) 10 $k 1 127.0.0.1 3001 1 127.0.0.1 3000 > /dev/null &
done
echo "$m noeuds malicieux lancé !"


wait
echo "Terminé !"
