CC = g++
LIBS = -lcrypto
FLAGS = -std=c++14 -Wall -Wextra -Wpedantic -g
HFILE = MerkleContainer.h
CFile = MerkleContainer.cpp
OFile = MerkleContainer.o

all: build_lib test main

build_lib: MerkleContainer.o

MerkleContainer.o: MerkleContainer.cpp MerkleContainer.h
	$(CC) $(FLAGS) -c MerkleContainer.cpp

test: test.o MerkleContainer.o
	$(CC) $(FLAGS) -o test test.o MerkleContainer.o $(LIBS) 

test.o: test.cpp MerkleContainer.h
	$(CC) $(FLAGS) -c test.cpp

main: build_lib main.o
	$(CC) $(FLAGS) -o main main.o MerkleContainer.o $(LIBS)

main.o: main.cpp MerkleContainer.h
	$(CC) $(FLAGS) -c main.cpp -o main.o

clean:
	rm -f *.o test main testlog.txt

rebuild: clean all
