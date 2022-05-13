#! /bin/bash

#remove added file
rm -rf bootstrap_server;
rm -rf bin;
#remove host ip adresses file
rm -f hosts

docker container kill $(docker container ls -q);