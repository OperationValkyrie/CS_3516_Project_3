Jonathan Chang
CS 3516 A18 A01

To compile the project use the following two commands:
	make clean
	make all
	
To run the project use the following command:
	./p3

    At trace level 1, the simulator should display the packet recieved, 
    the source of the packets, and the final neighbor costs of the node.
    With this implimintation, it is impossible to only print the final costs,
    so a user must read from the bottom up to find the final results. A run on
    the default configuration is shown in output.pdf with the final costs highlighted.

    At trace level 3, the simulator will also print out the distance table for each node
    this may or may not be required.

    This program was developed directly on the ccc machines.