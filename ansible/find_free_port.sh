#! /bin/bash

CHECK="do while"

while [[ ! -z $CHECK ]]; do
    PORT=$(( ( RANDOM % 60000 )  + 1025 ))
    CHECK=$(netstat -ap | grep $PORT)
done

echo $PORT
