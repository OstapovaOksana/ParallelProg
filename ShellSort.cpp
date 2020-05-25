#include <stdlib.h>
#include <ctime>
#include <iostream>
using namespace std;

#define N 50000000
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

void shellSort(int a[], int n)
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
	int* vector = (int*)malloc(N * sizeof(int)); 
	int i;

	srand((unsigned int)(NULL));
	for (i = 0; i < N; i++) {
		vector[i] = rand() % N;		
	}
	//cout << "Array before sorting: \n";
	//printArray(vector, N);
	start_time = clock();
	shellSort(vector, N);
	//cout << "\nArray after sorting: \n";
	//printArray(vector, N);
	end_time = clock();
	cout << "\nSerial sort time: " << (end_time - start_time) / CLOCKS_PER_SEC << " seconds." << endl;
	return 0;
}