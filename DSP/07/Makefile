CC  =gcc

07ravi.out: minpriorityqueue.o graph.o  experiment.o
	$(CC)  minpriorityqueue.o graph.o  experiment.o -o 07ravi.out

experiment.o: experiment.c
	$(CC) -c -ggdb experiment.c

graph.o: graph.h graph.c
	$(CC) -c -ggdb graph.c

minpriorityqueue.o: minpriorityqueue.h minpriorityqueue.c
	$(CC) -c -ggdb minpriorityqueue.c



clean:
	rm *.o *.out *~
