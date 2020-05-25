#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include "ShellSortMPI.h"
#include <iostream>
using namespace std;

#define N 50000000
double start_time;
double end_time;

void shellsort(int numbers[], int length)
{
	int i, j, increment, temp;
	increment = N/2;
	while (increment > 0)
	{
		for (i = 0; i < length; i++)
		{
			j = i;
			temp = numbers[i];
			while ((j >= increment) && (numbers[j - increment] > temp))
			{
				numbers[j] = numbers[j - increment];
				j = j - increment;
			}
			numbers[j] = temp;
		}
		increment = increment / 2;
	}
}

void printArray(int arr[], int n)
{
	for (int i = 0; i < n; i++)
		cout << arr[i] << " ";
}

int main(int argc, char** argv)
{
	int* data{};
	int* chunk;
	int* other;
	int m, n = N;
	int ProcNum; // Number of available processes
	int ProcRank; // Rank of current process 
	int s;
	int i;
	int step;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	printf("Number of available processes = %d \n", ProcNum);
	printf("Rank of current process = %d \n", ProcRank);

	if (ProcRank == 0)
	{
		int r;
		s = n / ProcNum;
		r = n % ProcNum;
		data = (int*)malloc((n + ProcNum - r) * sizeof(int));
		srand((unsigned int)(NULL));
		for (i = 0; i < n; i++)
			data[i] = rand() % n;
		if (r != 0)
		{
			for (i = n; i < n + ProcNum - r; i++)
				data[i] = 0;
			s = s + 1;
		}
		start_time = clock();

		MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
		chunk = (int*)malloc(s * sizeof(int));
		MPI_Scatter(data, s, MPI_INT, chunk, s, MPI_INT, 0, MPI_COMM_WORLD);
		shellsort(chunk, s);
	}
	else
	{
		MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
		chunk = (int*)malloc(s * sizeof(int));
		MPI_Scatter(data, s, MPI_INT, chunk, s, MPI_INT, 0, MPI_COMM_WORLD);
		shellsort(chunk, s);
	}

	if (ProcRank == 0)
	{
		end_time = clock();
		printf("MPI sort time: %f seconds\n", (end_time - start_time) / CLOCKS_PER_SEC);
		//printArray(chunk, s);
	}
	MPI_Finalize();
}