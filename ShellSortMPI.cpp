#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include "ShellSortMPI.h"
#include <iostream>
using namespace std;

#define N 5000000
double start_time;
double end_time;

void insertionsort(int a[], int n, int step) {
	for (int j = step; j < n; j += step) {
		int key = a[j];
		int i = j - step;
		while (i >= 0 && a[i] > key) {
			a[i + step] = a[i];
			i -= step;
		}
		a[i + step] = key;
	}
}

void shellsort(int a[], int n)
{
	int i, m;

	for (m = n / 2; m > 0; m /= 2)
	{
		for (i = 0; i < m; i++)
			insertionsort(&(a[i]), n - i, m);
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
	int* result{};
	int m, n = N;
	int ProcNum; // �-��� ������ �������
	int ProcRank; // ���� ��������� ������� 
	int s;
	int i;
	int step;
	MPI_Status status;

	MPI_Init(&argc, &argv); //�-� ����������� ���������� ��������� ��������
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank); // ������� ���� �������
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum); // ������� �-��� ������� � ���������� ���������� �������
											// MPI_COMM_WORLD - ���������� ��� ���������� ���������� ����� �������
	
	printf("Number of available processes = %d \n", ProcNum);
	printf("Rank of current process = %d \n", ProcRank);

	if (ProcRank == 0)
	{
		int r;
		s = n / ProcNum; // �-��� ������� ��������� �� ����� ������
		r = n % ProcNum;
		data = (int*)malloc((n + ProcNum - r) * sizeof(int));
		result = (int*)malloc(ProcNum * s * sizeof(int));
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

		MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD); // ���� ���� �� �-��� ������� �� ����� ������ ���������
													// ����� �������� �� �������� ����� ��������
													// ��� ����� ��������������� ������� ����������� �������� �� ������ ������� ���������
		chunk = (int*)malloc(s * sizeof(int));
		MPI_Scatter(data, s, MPI_INT, chunk, s, MPI_INT, 0, MPI_COMM_WORLD); // ������� ��� �� ����� ���������� ������
																			// � ������� �� ����� ��������
																			// data - �������������, chunc - ��, �� ������ ������
		shellsort(chunk, s);
		MPI_Gather(chunk, s, MPI_INT, result, s, MPI_INT, 0 ,MPI_COMM_WORLD); // ������ ���������� � ������
	}
	else
	{
		MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
		chunk = (int*)malloc(s * sizeof(int));
		MPI_Scatter(data, s, MPI_INT, chunk, s, MPI_INT, 0, MPI_COMM_WORLD);
		shellsort(chunk, s);
		MPI_Gather(chunk, s, MPI_INT, result, s, MPI_INT, 0, MPI_COMM_WORLD);
	}

	if (ProcRank == 0)
	{
		end_time = clock();
		printf("MPI sort time: %f seconds\n", (end_time - start_time) / CLOCKS_PER_SEC);
		//printArray(chunk, s);
	}
	MPI_Finalize();
}