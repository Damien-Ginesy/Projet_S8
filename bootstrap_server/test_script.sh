./bootstrap_server eclipse 1 inst 2 24 10 &

./client localhost 8080 0 2 > /dev/null
./client localhost 8080 0 3 > /dev/null
./client localhost 8080 0 2 > /dev/null
./client localhost 8080 0 2 > /dev/null
./client localhost 8080 1 2 > /dev/null
./client localhost 8080 1 2 > /dev/null
./client localhost 8080 2 2 > /dev/null
./client localhost 8080 2 2 > /dev/null
./client localhost 8080 2 2 > /dev/null
./client localhost 8080 2 2 > /dev/null
