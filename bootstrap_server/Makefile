CFLAGS=-g -Wall -std=gnu99
LDFLAGS=-lpthread

all: bootstrap_server

bootstrap_server : bootstrap_server.c bootstrap.o ip.o socket_IO.o
	gcc $(CFLAGS) -o bootstrap_server bootstrap_server.c bootstrap.o ip.o socket_IO.o

node : node Node.c socket_IO.c socket_IO.h
	gcc $(CFLAGS) -o node Node.c socket_IO.c socket_IO.h

clean :
	rm -f bootstrap_server Node *.o