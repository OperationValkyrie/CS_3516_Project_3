#include <stdio.h>
#include <stdlib.h>
#include "project3.h"

extern int TraceLevel;
extern float clocktime;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};

// Structs
static struct distance_table *dt0;
static struct NeighborCosts  *neighbor0;

// Functions
static void printdt0(int MyNodeNumber, struct NeighborCosts *neighbor, 
		struct distance_table *dtptr );
static void clearDT0(struct distance_table *dtprt);
static int determineMinCost0(int node, int new, struct distance_table *dtptr, struct NeighborCosts *neighbor);
static void sendupdate0(int node, unsigned int connected[MAX_NODES], struct distance_table *dtptr);
static void copyArray0(int dest[MAX_NODES], struct distance_table *dtptr, int node);
static void printCosts0(int node, char message[], int costs[MAX_NODES]);

/* students to write the following two routines, and maybe some others */

static unsigned int node_num = 0;;
static unsigned int connected0[MAX_NODES] = {0};
static unsigned int outputLevel = 3;

/**
 * Initializes the nodes and sends out the first packet
 */ 
void rtinit0() {
	dt0 = malloc(sizeof(struct distance_table));
    clearDT0(dt0);

	neighbor0 = getNeighborCosts(node_num);

	int i;
	for(i = 0; i < MAX_NODES; i++) {
		dt0->costs[i][node_num] = neighbor0->NodeCosts[i];
		if(neighbor0->NodeCosts[i] != INFINITY && i != node_num) {
			connected0[i] = 1;
		}
	}
    if(TraceLevel >= 1) {
        printCosts0(node_num, "has neighbor costs:", neighbor0->NodeCosts);
    }
    if(TraceLevel >= outputLevel) {
        printdt0(node_num, neighbor0, dt0);
    }
    sendupdate0(node_num, connected0, dt0);
}

/**
 * When recieve a packet, update distance table and send updates to neighbors if a change happens
 * @param *rcvdpkt The packet with the neighbor's information 
 */
void rtupdate0(struct RoutePacket *rcvdpkt) {
    if(TraceLevel >= 1) {
        printf("%s %f, %s %d %22s %d\n", "At time", clocktime, "node", node_num, "receives packet from node", rcvdpkt->sourceid);
        printCosts0(node_num, "receives packet with:", rcvdpkt->mincost);
    }
    int i;
    for(i = 0; i < MAX_NODES; i++) {
        dt0->costs[i][rcvdpkt->sourceid] = rcvdpkt->mincost[i];
    }
    int flag = determineMinCost0(node_num, rcvdpkt->sourceid, dt0, neighbor0);
    if(flag) {
        sendupdate0(node_num, connected0, dt0);
    }
    if(TraceLevel >= 1) {
        printCosts0(node_num, "now has min costs:", neighbor0->NodeCosts);
    }
    if(TraceLevel >= outputLevel) {
        printdt0(node_num, neighbor0, dt0);
    }
}

static int determineMinCost0(int node, int new, struct distance_table *dtptr, struct NeighborCosts *neighbor) {
    int flag = 0;
    int i;
    for(i = 0; i < MAX_NODES; i++) {
        int min = dtptr->costs[i][node];
        int j;
        for(j = 0; j < MAX_NODES; j++) {
            if(j == node) {
                continue;
            }
            int temp = dtptr->costs[new][node] + dtptr->costs[i][new];
            if(min > temp) {
                min = temp;
                flag = 1;
            }
        }
        dtptr->costs[i][node] = min;
        neighbor->NodeCosts[i] = min;
    }
    return flag;
}

static void sendupdate0(int node, unsigned int connected[MAX_NODES], struct distance_table *dtptr) {
    int i;
    for(i = 0; i < MAX_NODES; i++) {
        if(connected[i]) {
            struct RoutePacket *pkt0 = malloc(sizeof(struct RoutePacket));;
            pkt0->sourceid = node_num;
            pkt0->destid = i;
            copyArray0(pkt0->mincost, dtptr, node);
            toLayer2(*pkt0);
        }
    }
}

static void clearDT0(struct distance_table *dtptr) {
    int i;
    int j;
    for(i = 0; i < MAX_NODES; i++) {
        for(j = 0; j < MAX_NODES; j++) {
            dtptr->costs[i][j] = INFINITY;
        }
    }
}

static void copyArray0(int dest[MAX_NODES], struct distance_table *dtptr, int node) {
    int i;
    for(i = 0; i < MAX_NODES; i++) {
        dest[i] = dtptr->costs[i][node];
    }
}

static void printCosts0(int node, char message[], int costs[MAX_NODES]) {
    printf("%s %f, %s %d %22s", "At time", clocktime, "node", node_num, message);
    int i;
    for(i = 0; i < MAX_NODES; i ++) {
        printf(" %d", costs[i]);
    }
    printf("\n");
}
/////////////////////////////////////////////////////////////////////
//  printdt
//  This routine is being supplied to you.  It is the same code in
//  each node and is tailored based on the input arguments.
//  Required arguments:
//  MyNodeNumber:  This routine assumes that you know your node
//                 number and supply it when making this call.
//  struct NeighborCosts *neighbor:  A pointer to the structure 
//                 that's supplied via a call to getNeighborCosts().
//                 It tells this print routine the configuration
//                 of nodes surrounding the node we're working on.
//  struct distance_table *dtptr: This is the running record of the
//                 current costs as seen by this node.  It is 
//                 constantly updated as the node gets new
//                 messages from other nodes.
/////////////////////////////////////////////////////////////////////
static void printdt0( int MyNodeNumber, struct NeighborCosts *neighbor, 
		struct distance_table *dtptr ) {
    int       i, j;
    int       TotalNodes = neighbor->NodesInNetwork;     // Total nodes in network
    int       NumberOfNeighbors = 0;                     // How many neighbors
    int       Neighbors[MAX_NODES];                      // Who are the neighbors

    // Determine our neighbors 
    for ( i = 0; i < TotalNodes; i++ )  {
        if (( neighbor->NodeCosts[i] != INFINITY ) && i != MyNodeNumber )  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }
    // Print the header
    printf("                via     \n");
    printf("   D%d |", MyNodeNumber );
    for ( i = 0; i < NumberOfNeighbors; i++ )
        printf("     %d", Neighbors[i]);
    printf("\n");
    printf("  ----|-------------------------------\n");

    // For each node, print the cost by travelling thru each of our neighbors
    for ( i = 0; i < TotalNodes; i++ )   {
        if ( i != MyNodeNumber )  {
            printf("dest %d|", i );
            for ( j = 0; j < NumberOfNeighbors; j++ )  {
                    printf( "  %4d", dtptr->costs[i][Neighbors[j]] );
            }
            printf("\n");
        }
    }
    printf("\n");
}    // End of printdt0

