CC = g++
LIBS = -lcrypto
FLAGS = -std=c++14 -Wall -Wextra -Wpedantic -g

all: build_lib test main

build_lib: MerkleTree.o

MerkleTree.o: MerkleTree.cpp MerkleTree.h
	$(CC) $(FLAGS) -c MerkleTree.cpp

test: test.o MerkleTree.o
	$(CC) $(FLAGS) -o test test.o MerkleTree.o $(LIBS) 

test.o: test.cpp MerkleTree.h
	$(CC) $(FLAGS) -c test.cpp

main: build_lib main.o
	$(CC) $(FLAGS) -o main main.o MerkleTree.o $(LIBS)

main.o: main.cpp MerkleTree.h
	$(CC) $(FLAGS) -c main.cpp -o main.o

clean:
	rm -f *.o test main testlog.txt

rebuild: clean all
