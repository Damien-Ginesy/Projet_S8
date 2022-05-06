#!/bin/bash

echo "Lancement..."
v=10
m=50
for i in $(seq 1 $m)
do
#gnome-terminal -- run
./bin/p2p_test $((3000 + $i)) $v 127.0.0.1:8080 127.0.0.1:8081> /dev/null &
done
echo "$m noeuds lancé !"
wait
echo "Terminé !"
