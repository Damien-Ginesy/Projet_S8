CFLAGS=-Wall -std=gnu99
LDFLAGS=-lpthread

all: bootstrap_server

bootstrap_server : bootstrap_server.c
	gcc $(CFLAGS) -o bootstrap_server bootstrap_server.c

socket_IO.o : socket_IO.c socket_IO.h
	gcc $(CFLAGS) -o socket_IO.o -c socket_IO.c

ip.o : ip.c ip.h
	gcc $(CFLAGS) -o ip.o -c ip.c

bootstrap.o : bootstrap.c bootstrap.h
	gcc $(CFLAGS) -o bootstrap.o -c bootstrap.c

test : test.c ip.o bootstrap.o
	gcc $(CFLAGS) -o test test.c ip.o bootstrap.o

clean :
	rm -f bootstrap_server test *.o