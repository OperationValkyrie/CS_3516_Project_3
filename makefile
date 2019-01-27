CFLAGS = -c -g -Wall

all: p3

p3: project3.o node0.o node1.o node2.o node3.o
	gcc -g project3.c node0.c node1.c node2.c node3.c -o p3

project3.o: project3.c
	gcc $(CFLAGS) project3.c
	
node0.o: node0.c
	gcc $(CFLAGS) node0.c
	
node1.o: node1.c
	gcc $(CFLAGS) node1.c
	
node2.o: node2.c
	gcc $(CFLAGS) node2.c
	
node3.o: node3.c
	gcc $(CFLAGS) node3.c
	
clean:
	rm -f *.o p3