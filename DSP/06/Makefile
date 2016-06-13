CC = gcc

all: Node.o BTree.o Experiment.o
	$(CC) -Wall Node.o BTree.o Experiment.o -o ravi.out
Node.o:
	$(CC) -Wall -c Node.c -o Node.o
BTree.o:
	$(CC) -Wall -c BTree.c -o BTree.o
Experiment.o:
	$(CC) -Wall -c Experiment.c -o Experiment.o
clean:
	rm *.o *.out
