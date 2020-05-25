#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <omp.h>
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

void shellsort(int a[], int n)
{
    int i, m;

    for (m = n / 2; m > 0; m /= 2)
    {
    #pragma omp parallel for shared(a,m,n) private (i)
        for (i = 0; i < m; i++)
            insertionsort(&(a[i]), n - i, m);
    }
}

int main(int argc, char** argv) {
    omp_set_num_threads(4);
    const int n = N;
    int* data;
    int missorted;

    data = (int*)malloc(n * sizeof(int));

    srand((unsigned int)(NULL));
    for (int i = 0; i < n; i++) {
        data[i] = rand() % n;
    }

    start_time = clock();
    shellsort(data, n);
    end_time = clock();

    printf("OpenMP sort time: %f seconds\n", (end_time - start_time) / CLOCKS_PER_SEC);

    return 0;
}

//void shellSort(int numbers[], int length)
//{
//	int i, j, increment, temp;
//	increment = N / 2;
//	while (increment > 0)
//	{
//#pragma omp parallel
//		{
//#pragma omp for
//				for (i = 0; i < length; i++)
//				{
//					j = i;
//					temp = numbers[i];
//					while ((j >= increment) && (numbers[j - increment] > temp))
//					{
//						numbers[j] = numbers[j - increment];
//						j = j - increment;
//					}
//					numbers[j] = temp;
//				}
//		}
//		
//		increment = increment / 2;
//
//	}
//
//}
//
//void printArray(int arr[], int n)
//{
//	for (int i = 0; i < n; i++)
//		cout << arr[i] << " ";
//}
//
//int main(int argc, char** argv)
//{
//	//omp_set_num_threads(64);
//	int* vector = (int*)malloc(N * sizeof(int));
//	int i;
//
//	srand((unsigned int)(NULL));
//	for (i = 0; i < N; i++) {
//		vector[i] = rand() % N;
//	}
//	//cout << "Array before sorting: \n";
//	//printArray(vector, N);
//	start_time = clock();
//	shellSort(vector, N);
//	//cout << "\nArray after sorting: \n";
//	//printArray(vector, N);
//	end_time = clock();
//	cout << "\nOpenMP sort time: " << (end_time - start_time) / CLOCKS_PER_SEC << " seconds." << endl;
//	return 0;
//}