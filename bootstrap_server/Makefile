CFLAGS=-Wall -std=gnu99
LDFLAGS=-lpthread

all: bootstrap_server

bootstrap_server : bootstrap_server.c bootstrap.o ip.o socket_IO.o
	gcc $(CFLAGS) -o bootstrap_server bootstrap_server.c bootstrap.o ip.o socket_IO.o

socket_IO.o : socket_IO.c socket_IO.h
	gcc $(CFLAGS) -o socket_IO.o -c socket_IO.c

ip.o : ip.c ip.h
	gcc $(CFLAGS) -o ip.o -c ip.c

attack.o : attack.c attack.h
	gcc $(CFLAGS) -o attack.o -c attack.c

bootstrap.o : bootstrap.c bootstrap.h
	gcc $(CFLAGS) -o bootstrap.o -c bootstrap.c

test : test.c ip.o bootstrap.o attack.o
	gcc $(CFLAGS) -o test test.c ip.o bootstrap.o attack.o

clean :
	rm -f bootstrap_server test *.o