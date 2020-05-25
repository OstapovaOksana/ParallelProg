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

//int* merge(int* v1, int n1, int* v2, int n2)
//{
//	int i, j, k;
//	int* result;
//
//	result = (int*)malloc((n1 + n2) * sizeof(int));
//
//	i = 0; j = 0; k = 0;
//	while (i < n1 && j < n2)
//		if (v1[i] < v2[j])
//		{
//			result[k] = v1[i];
//			i++; k++;
//		}
//		else
//		{
//			result[k] = v2[j];
//			j++; k++;
//		}
//	if (i == n1)
//		while (j < n2)
//		{
//			result[k] = v2[j];
//			j++; k++;
//		}
//	else
//		while (i < n1)
//		{
//			result[k] = v1[i];
//			i++; k++;
//		}
//	return result;
//}
//
//void shellsort(int numbers[], int length)
//{
//	int i, j, increment, temp;
//	increment = 3;
//	while (increment > 0)
//	{
//		for (i = 0; i < length; i++)
//		{
//			j = i;
//			temp = numbers[i];
//			while ((j >= increment) && (numbers[j - increment] > temp))
//			{
//				numbers[j] = numbers[j - increment];
//				j = j - increment;
//			}
//			numbers[j] = temp;
//		}
//		increment = increment / 2;
//	}
//}
//
//int main(int argc, char** argv)
//{
//	int* data{};
//	int* chunk;
//	int* other;
//	int m, n = N;
//	int id, p;
//	int s;
//	int i;
//	int step;
//	MPI_Status status;
//
//	MPI_Init(&argc, &argv);
//	MPI_Comm_rank(MPI_COMM_WORLD, &id);
//	MPI_Comm_size(MPI_COMM_WORLD, &p);
//
//	if (id == 0)
//	{
//		int r;
//		s = n / p;
//		r = n % p;
//		data = (int*)malloc((n + p - r) * sizeof(int));
//		srand((unsigned int)(NULL));
//		for (i = 0; i < n; i++)
//			data[i] = rand() % n;
//		if (r != 0)
//		{
//			for (i = n; i < n + p - r; i++)
//				data[i] = 0;
//			s = s + 1;
//		}
//		start_time = clock();
//
//		MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
//		chunk = (int*)malloc(s * sizeof(int));
//		MPI_Scatter(data, s, MPI_INT, chunk, s, MPI_INT, 0, MPI_COMM_WORLD);
//		shellsort(chunk, s);
//	}
//	else
//	{
//		MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
//		chunk = (int*)malloc(s * sizeof(int));
//		MPI_Scatter(data, s, MPI_INT, chunk, s, MPI_INT, 0, MPI_COMM_WORLD);
//		shellsort(chunk, s);
//	}
//
//	step = 1;
//	while (step < p)
//	{
//		if (id % (2 * step) == 0)
//		{
//			if (id + step < p)
//			{
//				MPI_Recv(&m, 1, MPI_INT, id + step, 0, MPI_COMM_WORLD, &status);
//				other = (int*)malloc(m * sizeof(int));
//				MPI_Recv(other, m, MPI_INT, id + step, 0, MPI_COMM_WORLD, &status);
//				chunk = merge(chunk, s, other, m);
//				s = s + m;
//			}
//		}
//		else
//		{
//			int near = id - step;
//			MPI_Send(&s, 1, MPI_INT, near, 0, MPI_COMM_WORLD);
//			MPI_Send(chunk, s, MPI_INT, near, 0, MPI_COMM_WORLD);
//			break;
//		}
//		step = step * 2;
//	}
//	if (id == 0)
//	{
//		end_time = clock();
//		printf("MPI sort time: %f seconds\n", (end_time - start_time) / CLOCKS_PER_SEC);
//	}
//	MPI_Finalize();
//}

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