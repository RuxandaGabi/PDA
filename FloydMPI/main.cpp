#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

const int INFINITY = 9999;

void Read_matrix(int local_mat[], int n, int my_rank, int p, MPI_Comm comm)
{
	int i, j;
	int* temp_mat = NULL;
	FILE *f = fopen("file.txt", "r");
	if (my_rank == 0)
	{
		temp_mat = (int *)malloc(n*n * sizeof(int));
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				fscanf(f, "%d", &temp_mat[i*n + j]);
		MPI_Scatter(temp_mat, n*n / p, MPI_INT, local_mat, n*n / p, MPI_INT, 0, comm);
		free(temp_mat);
	}
	else
	{
		MPI_Scatter(temp_mat, n*n / p, MPI_INT, local_mat, n*n / p, MPI_INT, 0, comm);
	}
	fclose(f);
}

void Print_matrix(int local_mat[], int n, int my_rank, int p, MPI_Comm comm)
{
	int i, j;
	int* temp_mat = NULL;
	if (my_rank == 0)
	{
		temp_mat = (int *)malloc(n*n * sizeof(int));
		MPI_Gather(local_mat, n*n / p, MPI_INT, temp_mat, n*n / p, MPI_INT, 0, comm);
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
				if (temp_mat[i*n + j] == INFINITY)
					printf("-1 ");
				else
					printf("%d ", temp_mat[i*n + j]);
			printf("\n");
		}
		free(temp_mat);
	}
	else
	{
		MPI_Gather(local_mat, n*n / p, MPI_INT, temp_mat, n*n / p, MPI_INT, 0, comm);
	}
}

void Copy_row(int local_mat[], int n, int p, int row_k[], int k)
{
	int j;
	int local_k = k % (n / p);
	for (j = 0; j < n; j++)
		row_k[j] = local_mat[local_k*n + j];
}

void Floyd(int local_mat[], int n, int my_rank, int p, MPI_Comm comm)
{
	int global_k, local_i, global_j, temp, temp2;
	int root;
	int* row_k = (int *)malloc(n * sizeof(int));
	for (global_k = 0; global_k < n; global_k++)
	{
		root = global_k / (n / p);
		if (my_rank == root)
			Copy_row(local_mat, n, p, row_k, global_k);
		MPI_Bcast(row_k, n, MPI_INT, root, comm);
		for (local_i = 0; local_i < n / p; local_i++)
			for (global_j = 0; global_j < n; global_j++)
			{
				temp = local_mat[local_i*n + global_k] + row_k[global_j];
				temp2 = local_mat[local_i*n + global_j];
				if (temp < temp2)
					local_mat[local_i*n + global_j] = temp;
			}
	}
	free(row_k);
}

int main(int argc, char* argv[])
{
	int n;
	int* local_mat;
	int p, my_rank, x;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	n = 6;

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	local_mat = (int *)malloc(n*n / p * sizeof(int));

	Read_matrix(local_mat, n, my_rank, p, MPI_COMM_WORLD);

	Floyd(local_mat, n, my_rank, p, MPI_COMM_WORLD);

	if (my_rank == 0)
		printf("Solution :\n");
	Print_matrix(local_mat, n, my_rank, p, MPI_COMM_WORLD);

	free(local_mat);
	MPI_Finalize();

	return 0;
}
