ARGS=-g -Wall -std=c++17

all: bin main

%: %.cpp sha256.c Node.cpp joaat_hash.c
	g++ $^ $(ARGS) -o bin/$@

bin:
	mkdir bin