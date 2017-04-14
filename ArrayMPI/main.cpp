#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXSIZE 50

int main(int argc, char **argv)
{
	int id, numprocs;
	int arr[MAXSIZE], i, myresult = 0, result, elem = 13, x, low, high;
	FILE *fp;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

    fp = fopen("file.txt", "r");

    if(fp == NULL){
        return 0;
    }

    for(i = 0; i < MAXSIZE; i++){
        fscanf(fp, "%d", &arr[i]);
    }

	MPI_Bcast(arr, MAXSIZE, MPI_INT, 0, MPI_COMM_WORLD);

	x = MAXSIZE / numprocs;
	low = id * x;
	high = id + x;
	for(i = low; i < high; i++){
		if(arr[i] == elem){
			myresult = i + 1;
		}
	}

	MPI_Reduce(&myresult, &result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if(id == 0){
		printf("Maximum position: %d\n", result);
	}

	MPI_Finalize();
}
