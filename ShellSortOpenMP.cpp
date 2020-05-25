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