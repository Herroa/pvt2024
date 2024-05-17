#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <iostream>

#define threshold 1000

double wtime()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1E-9;
}

void partition(int *v, int &i, int &j, int low, int high)
{
    i = low;
    j = high;
    int pivot = v[(low + high) / 2];
    do
    {
        while (v[i] < pivot)
            i++;
        while (v[j] > pivot)
            j--;
        if (i <= j)
        {
            std::swap(v[i], v[j]);
            i++;
            j--;
        }
    } while (i <= j);
}

void quicksort(int *v, int low, int high)
{
    int i, j;
    partition(v, i, j, low, high);
    if (low < j)
        quicksort(v, low, j);
    if (i < high)
        quicksort(v, i, high);
}

void quicksort_tasks(int *v, int low, int high)
{
    int i, j;
    partition(v, i, j, low, high);
    if (high - low < threshold || (j - low < threshold || high - i < threshold))
    {
        if (low < j)
            quicksort_tasks(v, low, j);
        if (i < high)
            quicksort_tasks(v, i, high);
    }
    else
    {
#pragma omp task untied // Открепить задачу от потока (задачу может выполнять любой поток)
        {
            quicksort_tasks(v, low, j);
        }
        quicksort_tasks(v, i, high);
    }
}

int main()
{
    srand(time(NULL));
    FILE *fptr;
    fptr = fopen("data.dat", "w");
    int size_of_array = 1000000;
    int array[size_of_array];
    // serial
    for (int i = 0; i < size_of_array; i++)
    {
        array[i] = rand();
    }
    double time = -wtime();
    quicksort(array, 0, size_of_array - 1);
    time += wtime();
    double serial_time = time;
    printf("serial - %f\n", time);

    //parallel
    for (int i = 2; i <= 8; i += 2)
    {
        for (int i = 0; i < size_of_array; i++)
        {
            array[i] = rand();
        }

        time = -wtime();
#pragma omp parallel num_threads(i)
        {
#pragma omp single
            quicksort_tasks(array, 0, size_of_array - 1);
        }
        time += wtime();
        // for(int j = 0;j < size_of_array;j++)
        //     printf("%d ", array[j]);
        // printf("\n");
        fprintf(fptr, "%d   %f\n", i, serial_time/time);
    }
    fclose(fptr);
    return 0;
}