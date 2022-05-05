#!/bin/bash

echo "Lancement..."
v=10
m=50
for i in {1..50}
do
#gnome-terminal -- run
./bin/p2p_test $((3000 + $i)) $v 127.0.0.1:8080 > /dev/null &
done
echo "$m noeuds lancé !"
wait
echo "Terminé !"
