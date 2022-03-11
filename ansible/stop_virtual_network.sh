#! /bin/bash

#remove host ip adresses file
rm -f ip_adresses

docker container kill $(docker container ls -q);