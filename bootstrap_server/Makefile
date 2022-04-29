CFLAGS=-g -Wall -std=gnu99 -DDEBUG=1
LDFLAGS=-lpthread

all: bootstrap_server client

bootstrap_server : bootstrap_server.c exchange_protocol.h bootstrap.o ip.o socket_IO.o attack.o
	gcc $(CFLAGS) -o bootstrap_server bootstrap_server.c bootstrap.o ip.o socket_IO.o attack.o

client : Node.c exchange_protocol.h socket_IO.o ip.o
	gcc $(CFLAGS) -o client Node.c socket_IO.o ip.o

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
	rm -f bootstrap_server client test *.o *.report