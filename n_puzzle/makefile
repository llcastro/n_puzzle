all: n_puzzle clean

n_puzzle: n_puzzle.c queue.o binary_heap.o
	 gcc -Wall -g -o n_puzzle n_puzzle.c binary_heap.o queue.o

queue.o: ../queue/queue.c ../queue/queue.h
	gcc -c ../queue/queue.c ../queue/queue.h

binary_heap.o: ../binary_heap/binary_heap.c ../binary_heap/binary_heap.h
	gcc -c ../binary_heap/binary_heap.c ../binary_heap/binary_heap.h

clean:
	 rm *.o
