#! /bin/bash

# attack id
# node nbr
# view size
# cycles_before_reset
# nodes_per_reset
# cycles_per_second
# main_machain_ip
# bootstrap_port


i=0;
while [ "$i" -lt $2 ];
do
    i=$(expr $i + 1);
    
    port_store_filename=$((1 + $RANDOM))
    port="$(echo "basalt" | su root -c "/home/peer/find_free_port.sh > $port_store_filename" > /dev/null 2> /dev/null; cat $port_store_filename; rm $port_store_filename;)";

    if [ $1 -eq "0" ]; then
        /home/peer/bin/honest_node $3 $port $4 $5 $6 $7 $8 $7 3000 > /dev/null &
    else
        /home/peer/bin/malicious_node $3 $port $4 $5 $6 $7 $8 $1 $7 3000 > /dev/null &
    fi

done;