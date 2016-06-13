CC = gcc

all: node.o BST.o AVL.o SearchExperiment.o
	$(CC) -Wall node.o BST.o AVL.o SearchExperiment.o -o ravi.out
node.o:
	$(CC) -Wall -c node.c -o node.o
BST.o:
	$(CC) -Wall -c BST.c -o BST.o
AVL.o:
	$(CC) -Wall -c AVL.c -o AVL.o
SearchExperiment.o:
	$(CC) -Wall -c SearchExperiment.c -o SearchExperiment.o
clean:
	rm *.o *.out
