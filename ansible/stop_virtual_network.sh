#! /bin/bash

#remove added file
rm -r bootstrap_server;
#remove host ip adresses file
rm -f hosts

docker container kill $(docker container ls -q);