#! /bin/bash

pkill bootstrap_serve;

kill $(lsof -t -i :3000);