#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>
 
int main(int argc, char* argv[])
{
    long long size = 1000000;
    //size is sample size, we can alter this for the number of points.
    int myid, i, partition, count=0, totalValid, numprocs, remaining;
    //my id holds rank ID
    //i is a counting var
    //partition is how many numbers each node gets to randomly try.
    //count is for keeping track of valid points per node
    //totalValid is total good points from EVERY node
    //numprocs is number of nodes.
    //remaining is the remaining number of points left over after we divide the size up among all the nodes.
    double x, y, pi, timeBegin, timeEnd, elapsed, delta, realPi = 3.1415;
    //x and y are placeholders for the coordinates
    //pi is used to hold our final approx.
    //delta is used to hold the difference between real Pi and our approx.
    //realPi is used to get our delta.
 
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    partition = (size/numprocs);
    srand((unsigned)(myid));
    
    if (myid == 0)
    {
	timeBegin = MPI_Wtime();
	//printf("Time begin assigned: %f\n", timeBegin);
    } 

    for (i=0; i<partition; ++i)                  //main loop
    {
        x = (double)random()/(double)RAND_MAX;          //gets a random x coordinate
        y = (double)random()/(double)RAND_MAX;          //gets a random y coordinate
        //are they in the unit circle?
        if (sqrt((x*x)+(y*y)) <= 1)
        {
            ++count;                //if so, it is a valid point.
        }
    }

    //in case we miss some points due to remainders from the rounding
    if ( (myid == 0) && (size % numprocs != 0) )
    {
        remaining = size % numprocs;
        for (i=0; i<remaining; ++i)                  //main loop
        {
            //printf("entered loop\n");
	    x = (double)random()/(double)RAND_MAX;          //gets a random x coordinate
            y = (double)random()/(double)RAND_MAX;          //gets a random y coordinate
            //are they in the unit circle?
            if (sqrt((x*x)+(y*y)) <= 1)
            {
                ++count;                //if so, it is a valid point.
            }
        }
    }
 
    MPI_Reduce(&count, &totalValid, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
     
    if (myid == 0)                                                      //if root node
    {
	timeEnd = MPI_Wtime();
	//printf("Time end: %f\n", timeEnd);
	//printf("Time begin: %f\n", timeBegin);
	elapsed = timeEnd - timeBegin;
	//printf("real Pi: %f\n", realPi);
        pi = (((double)totalValid/(double)size))*4.0;                             		 //recall we multiple by 4 as the ratio of totalValid/size = pi/4.
        delta = pi - realPi;
	printf("Pi: %f Delta: %f Elapsed time: %f\n", pi, delta, elapsed);                                            //print final
    }
    MPI_Finalize();
    return 0;
}
