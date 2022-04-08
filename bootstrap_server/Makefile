CFLAGS=-Wall -std=gnu99
LDFLAGS=-lpthread

all: bootstrap_server

bootstrap_server : bootstrap_server.c
	gcc $(CFLAGS) -o bootstrap_server bootstrap_server.c

socket_IO.o : socket_IO.c socket_IO.h
	gcc $(CFLAGS) -o socket_IO.o -c socket_IO.c

clean :
	rm -f bootstrap_server *.o