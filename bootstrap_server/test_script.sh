./bootstrap_server eclipse 1 inst 2 24 10 &

./client localhost 8080 0 2 > 1.report &
./client localhost 8080 0 3 > 2.report &
./client localhost 8080 0 2 > 3.report &
./client localhost 8080 0 2 > 4.report &
./client localhost 8080 1 2 > 5.report &
./client localhost 8080 1 2 > 6.report &
./client localhost 8080 2 2 > 7.report &
./client localhost 8080 2 2 > 8.report &
./client localhost 8080 2 2 > 9.report &
./client localhost 8080 2 2 > 10.report &
