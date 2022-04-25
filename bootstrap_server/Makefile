CFLAGS=-g -Wall -std=gnu99
LDFLAGS=-lpthread

all: bootstrap_server

bootstrap_server : bootstrap.c bootstrap.h exchange_protocol.h bootstrap_server.c ip.c socket_IO.c socket_IO.h
	gcc $(CFLAGS) -o bootstrap_server bootstrap.c bootstrap.h exchange_protocol.h bootstrap_server.c ip.c socket_IO.c socket_IO.h

node : node Node.c socket_IO.c socket_IO.h
	gcc $(CFLAGS) -o node Node.c socket_IO.c socket_IO.h

clean :
	rm -f bootstrap_server Node *.o