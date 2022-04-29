CFLAGS=-g -Wall -std=gnu99 -DDEBUG=1
LDFLAGS=-lpthread

all: obj bin bootstrap_server client

bootstrap_server : bootstrap_server.c obj/bootstrap.o obj/ip.o obj/socket_IO.o obj/attack.o
	gcc $(CFLAGS) -o bin/$@ $?

client : Node.c obj/socket_IO.o obj/ip.o
	gcc $(CFLAGS) -o bin/$@ $?

socket_IO.o : socket_IO.c
	gcc $(CFLAGS) -o obj/$@ -c $?

ip.o : ip.c
	gcc $(CFLAGS) -o obj/$@ -c $?

attack.o : attack.c
	gcc $(CFLAGS) -o obj/$@ -c $?

bootstrap.o : bootstrap.c
	gcc $(CFLAGS) -o obj/$@ -c $?

test : test.c obj/ip.o obj/bootstrap.o obj/attack.o
	gcc $(CFLAGS) -o bin/$@ $?

clean :
	rm -f bootstrap_server bin/* obj/* *.report

obj:
	@mkdir obj

bin:
	@mkdir bin